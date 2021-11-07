
#include "pcb.h"

#include <modules/mpx_supt.h>
#include <include/string.h>
#include <lib/out.h>
#include <term/utils.h>
#include <term/args.h>
#include <term/dispatch/context.h>

/*
	We are going to have two queues for right now.

*/
pcb_queue_t p_queue;
pcb_queue_t f_queue;
pcb_queue_t * priority_queue = &p_queue;
pcb_queue_t * fifo_queue = &f_queue;


/********************************************************/
/****************** Backend stuff here ******************/
/********************************************************/

void initPCB() {
	   priority_queue->pcbq_count = 0;
	   priority_queue->pcbq_head = NULL;
	   priority_queue->pcbq_tail = NULL;
	   priority_queue->queue_order = PRIORITY;

	   fifo_queue->pcbq_count = 0;
	   fifo_queue->pcbq_head = NULL;
	   fifo_queue->pcbq_tail = NULL;
	   fifo_queue->queue_order = FIFO;
}

pcb_t * allocatePCB() {
	/* Initialize PCB */
	pcb_t *pcb = (pcb_t *) sys_alloc_mem(sizeof(pcb_t));

	if (pcb == NULL) {
		return NULL;
	}

	/* Beginning of the stack (BP) */
	pcb->pcb_stack_bottom = (unsigned char *) sys_alloc_mem(MAX_STACK_SIZE);
	if (pcb->pcb_stack_bottom == NULL) {
		return NULL;
	}

	/* End of the stack (SP) */
	pcb->pcb_stack_top = pcb->pcb_stack_bottom + MAX_STACK_SIZE - sizeof(context);

	/* Zero out memory in the stack frame (SF) */
	memset(pcb, MAX_STACK_SIZE, '\0');

	return pcb;
}

int freePCB(pcb_t * pcb) {
	// int free = sys_free_mem(pcb->pcb_stack_bottom);
	//int free = sys_free_mem(pcb);
	//return free;
	(void)pcb;
	return 0;
}

pcb_t * setupPCB(char * name, int process_class, int priority) {
	pcb_t *pcb = allocatePCB();
	
	if (pcb == NULL) {
		print("Something went wrong setting up PCB\n",35);
		return NULL;
	}

	strcpy(pcb->pcb_name, name);
	pcb->pcb_process_class = process_class;
	pcb->pcb_priority = priority;
	pcb->pcb_process_state = READY;

	return pcb;
}

pcb_t * findPCB(char * name) {
    /* Check for valid name */
    if (strlen(name) > MAX_NAME_SIZE || name == NULL) {
        printf("Error: Name of the PCB is too long\n");
        return NULL;
    }

    // Iterate through each PCB queues
    // Iterate through selected PCB queue
    pcb_node_t *cur_node = priority_queue->pcbq_head;
    while(cur_node != NULL) {
        if(strcmp(cur_node->pcb->pcb_name, name) == 0)
            return cur_node->pcb;
        cur_node = cur_node->pcbn_next_pcb;
    }
    cur_node = fifo_queue->pcbq_head;
    while(cur_node != NULL) {
        if(strcmp(cur_node->pcb->pcb_name, name) == 0)
            return cur_node->pcb;
        cur_node = cur_node->pcbn_next_pcb;
    }

    return NULL;
}

int insertPCB(pcb_t * pcb) {
	if (pcb == NULL) {
		return 1;
	}

	pcb_queue_t *queue;
	switch (pcb->pcb_process_state) {
		case READY:
		case SUSPENDED_READY:
			queue = priority_queue;
			break;
		case BLOCKED:
		case SUSPENDED_BLOCKED:
			queue = fifo_queue;
			break;
		default:
			return 1;
	}

	if(queue->pcbq_head == NULL) {
		// queue is empty - set this pcb as head and tail
		pcb_node_t *inserted_node = (pcb_node_t *)sys_alloc_mem(sizeof(pcb_node_t));
		// null next and prev nodes for new node
		inserted_node->pcbn_next_pcb = NULL;
		inserted_node->pcbn_prev_pcb = NULL;
		inserted_node->pcb = pcb;
		queue->pcbq_head = inserted_node;
		queue->pcbq_tail = inserted_node;
		return 0;
	}

	pcb_node_t *node;
	if(queue->queue_order == PRIORITY) {

		// PRIORITY queue - insert after all pcbs of greater or equal priority and before all pcbs of lesser priority
		node = queue->pcbq_head;
		if(node->pcb->pcb_priority < pcb->pcb_priority) {
			// node is replacing queue's current head
			pcb_node_t *inserted_node = (pcb_node_t *)sys_alloc_mem(sizeof(pcb_node_t));
			inserted_node->pcbn_next_pcb = node;
			inserted_node->pcbn_prev_pcb = NULL;
			inserted_node->pcb = pcb;
			node->pcbn_prev_pcb = inserted_node;
			queue->pcbq_head = inserted_node;
			return 0;
		}
		while(node->pcbn_next_pcb != NULL && node->pcbn_next_pcb->pcb->pcb_priority >= pcb->pcb_priority) {
			node = node->pcbn_next_pcb;
		}
	} else {
		// FIFO - insert at end
		node = queue->pcbq_tail;
	}
	
	// doubly linked lists sure are fun
	pcb_node_t *inserted_node = (pcb_node_t *)sys_alloc_mem(sizeof(pcb_node_t));
	inserted_node->pcbn_next_pcb = node->pcbn_next_pcb;
	inserted_node->pcbn_prev_pcb = node;
	inserted_node->pcb = pcb;
	node->pcbn_next_pcb = inserted_node;
	if(inserted_node->pcbn_next_pcb != NULL) {
		// node was inserted somewhere in middle of queue - need to update next node's prev pointer
		inserted_node->pcbn_next_pcb->pcbn_prev_pcb = inserted_node;
	} else {
		// node was inserted at end of queue - need to update queue's tail
		queue->pcbq_tail = inserted_node;
	}
	return 0;
}

int removePCB(pcb_t * pcb) {
	if (pcb == NULL) {
		return 1;
	}

	pcb_queue_t *queue;
	switch (pcb->pcb_process_state) {
		case READY:
		case SUSPENDED_READY:
			queue = priority_queue;
			break;
		case BLOCKED:
		case SUSPENDED_BLOCKED:
			queue = fifo_queue;
			break;
		default:
			return 1;
	}

	pcb_node_t *node = queue->pcbq_head; // the node to remove
	while(node != NULL) {
		if(node->pcb == pcb)
			break;
		node = node->pcbn_next_pcb;
	}
	if(node == NULL)
		return 1; // queue but is supposed to contain this PCB but doesn't - this shouldn't happen, but if it does it's an error

	if(node->pcbn_prev_pcb != NULL) {
		// node being removed had at least 1 node before it
		node->pcbn_prev_pcb->pcbn_next_pcb = node->pcbn_next_pcb;
	} else {
		// node being removed was head - adjust queue head pointer
		queue->pcbq_head = node->pcbn_next_pcb;
	}

	if(node->pcbn_next_pcb != NULL) {
		// node being removed had at least 1 node after it
		node->pcbn_next_pcb->pcbn_prev_pcb = node->pcbn_prev_pcb;
	} else {
		// node being removed was tail - adjust queue tail pointer
		queue->pcbq_tail = node->pcbn_prev_pcb;
	}
	
	// node is no longer needed and is not accessible - node can be freed
	serial_println("about to call sys_free_mem in removePCB");
	sys_free_mem(node);
	printf_debug("just returned from problematic sys_free_mem call\n");


	return 0;
}

/********************************************************/
/*************** User Command stuff here ****************/
/********************************************************/

int createPCB(char * args) {
	// The format of createpcb is : pcb_name.pcb_process_class.pcb_priority
	// So like... first_pcb_block.0.3

	char * name;
	char * token;
	int priority, params = 1;
	int process_class;

	skip_ws(&args);
	token = strtok(args,".");
	name = token;

	/** Parse the user input **/
	while (token != NULL) {
		token = strtok(NULL,".");

		switch (params) {
			case 1:
				process_class = atoi(token);
				break;
			case 2:
				priority = atoi(token);
				break;
			default:
				break;
		}

		params++;
	}

	/** Error Handling **/
	/* Check for correct number of parameters */
	if (params == 1) {
		print("Usage: createpcb [NAME.PROCESS_CLASS.PRIORITY]\n",47);
		return 1;
	}
	else if (params != 4) {
		print("Error: Wrong amount of parameters\n",27);
		return 1;
	}

	/* Check name for size and whether it exists in a PCB already */
	if (strlen(name) > MAX_NAME_SIZE || name == NULL) {
		print("Error: Name of the PCB is too long\n",35);
		return 1;
	} 
	else if (findPCB(name) != NULL) {
		print("Error: PCB with that name already exists\n",41);
		return 1;
	} 

	/* Check whether priority is out of bounds */
	if (priority > MAX_PRIORITY || priority < MIN_PRIORITY) {
		print("Error: Specified priority is out of bounds\n",43);
		return 1;
	}

	if (process_class != 0 && process_class != 1) {
		printf("Error: Specified process class does not exist\n"); // if the user doesn't enter a numeric value for the process class, this would print garbage data instead of what they entered, so I removed the part of this error message that echos what the user entered
		return 1;
	}
	
	
	/* Create the PCB */
	pcb_t * pcb = setupPCB(name, process_class, priority);
	
	/* Insert into PCB queue */
	insertPCB(pcb);
	
	return 0;
}


int setPriority(char *args) {
	skip_ws(&args);

	char * name;
	char * token;
	int priority, params = 1;

	token = strtok(args,".");
	name = token;

	/** Parse the user input **/
	while (token != NULL) {
		token = strtok(NULL,".");

		switch (params) {
			case 1:
				priority = atoi(token);
				break;
			default:
				break;
		}

		params++;
	}

	/** Error Handling **/
	/* Check for correct number of parameters */
	if (params != 3) {
		print("Error: Wrong amount of parameters\n",27);
		return 1;
	}

	/* Check whether priority is out of bounds */
	if (priority > MAX_PRIORITY || priority < MIN_PRIORITY) {
		print("Error: Specified priority is out of bounds\n",43);
		return 1;
	}

	/* Find PCB */
	pcb_t * pcb = findPCB(name);
	if (pcb == NULL) {
		printf("Error: Specified PCB does not exist\n");
		return 1;
	}

	/* Dispose of the old PCB */
	removePCB(pcb);

	/* Reinsert PCB with new priority */
	pcb->pcb_priority = priority;
	insertPCB(pcb);


	return 0;
}


int showPCB(char *args) {
	char *pcb_name;

	parsed_args *parsed_args = parse_args(args);
	if(parsed_args == NULL)
		return 1;
	if(!next_unnamed_arg(parsed_args, &pcb_name)) {
		println("Bad usage: PCB name not provided", 32);
		sys_free_mem(parsed_args);
		return 1;
	}

	pcb_t *pcb = findPCB(pcb_name);
	if(pcb == NULL) {
		println("Error: PCB not found", 20);
		sys_free_mem(parsed_args);
		return 1;
	}
	sys_free_mem(parsed_args);

	printf("Process %s - ", pcb->pcb_name);
	switch(pcb->pcb_process_state) {
		case RUNNING:
			display_fg_color(GREEN);
			printf("RUNNING\n");
			display_reset();
			break;
		case READY:
			display_fg_color(GREEN);
			printf("READY");
			display_reset();
			printf(" (not suspended)\n");
			break;
		case BLOCKED:
			display_fg_color(YELLOW);
			printf("BLOCKED");
			display_reset();
			printf(" (not suspended)\n");
			break;
		case SUSPENDED_READY:
			display_fg_color(RED);
			printf("READY");
			display_reset();
			printf(" (");
			display_fg_color(RED);
			printf("SUSPENDED");
			display_reset();
			printf(")\n");
			break;
		case SUSPENDED_BLOCKED:
			display_fg_color(RED);
			printf("BLOCKED");
			display_reset();
			printf(" (");
			display_fg_color(RED);
			printf("SUSPENDED");
			display_reset();
			printf(")\n");
			break;
	}
	
	printf("   Proc ID: %i\n", 0); // TODO: placeholder until/if process id is added
	printf("     Class: %s\n", pcb->pcb_process_class == 0 ? "SYS_PROCESS" : "APPLICATION");
	char *priority_str;
	if(pcb->pcb_priority < 3)
		priority_str = "LOW";
	else if(pcb->pcb_priority < 6)
		priority_str = "MEDIUM";
	else if(pcb->pcb_priority < 8)
		priority_str = "HIGH";
	else if(pcb->pcb_priority < 9)
		priority_str = "CRITICAL";
	else
		priority_str = "MAXIMUM";
	printf("  Priority: %i [%s]\n", pcb->pcb_priority, priority_str);

	return 0;
}

int showReady(char * p) {
	(void) p;

	/* Get head of priority queue */
	pcb_node_t * node = priority_queue->pcbq_head;
	if (node == NULL) {
		print("Ready Queue is empty\n",21);
		return 1;
	}

	/* Show each pcb node */
	while (node != NULL) {
		showPCB(node->pcb->pcb_name);
		printf("\n");
		node = node->pcbn_next_pcb;
	}

	return 0;
}

int showBlocked(char *args) {
	(void)args;

	pcb_node_t *node = fifo_queue->pcbq_head;
	if(node == NULL) {
		printf("No blocked PCBs found\n");
		return 0;
	}
	while(node != NULL) {
		showPCB(node->pcb->pcb_name);
		printf("\n");
		node = node->pcbn_next_pcb;
	}
	return 0;
}

int showAll(char *args) {
	(void)args;
	// TODO: also show the running proc
	showReady(NULL);
	showBlocked(NULL);
	return 0;
}

int suspendPCB(char *args) {
	char *pcb_name;

	parsed_args *parsed_args = parse_args(args);
	if(parsed_args == NULL)
		return 1;
	if(!next_unnamed_arg(parsed_args, &pcb_name)) {
		printf("Bad usage: PCB name not provided\n");
		sys_free_mem(parsed_args);
		return 1;
	}

	pcb_t *pcb = findPCB(pcb_name);
	if(pcb == NULL) {
		printf("Error: PCB not found\n");
		sys_free_mem(parsed_args);
		return 1;
	}
	sys_free_mem(parsed_args);

	removePCB(pcb);
	switch(pcb->pcb_process_state) {
		case RUNNING:
		case READY:
		case SUSPENDED_READY:
			pcb->pcb_process_state = SUSPENDED_READY;
			break;
		case BLOCKED:
		case SUSPENDED_BLOCKED:
			pcb->pcb_process_state = SUSPENDED_BLOCKED;
			break;
		default:
			return 1;
	}
	return insertPCB(pcb);
}

int resumePCB(char *args) {
	char *pcb_name;

	parsed_args *parsed_args = parse_args(args);
	if(parsed_args == NULL)
		return 1;
	if(!next_unnamed_arg(parsed_args, &pcb_name)) {
		printf("Bad usage: PCB name not provided\n");
		sys_free_mem(parsed_args);
		return 1;
	}

	pcb_t *pcb = findPCB(pcb_name);
	if(pcb == NULL) {
		printf("Error: PCB not found\n");
		sys_free_mem(parsed_args);
		return 1;
	}
	sys_free_mem(parsed_args);

	removePCB(pcb);
	switch(pcb->pcb_process_state) {
		case READY:
		case SUSPENDED_READY:
			pcb->pcb_process_state = READY;
			break;
		case BLOCKED:
		case SUSPENDED_BLOCKED:
			pcb->pcb_process_state = BLOCKED;
			break;
		case RUNNING:
			pcb->pcb_process_state = RUNNING;
			break;
		default:
			return 1;
	}
	return insertPCB(pcb);
}

int deletePCB(char *args) {
	char *pcb_name;

	parsed_args *parsed_args = parse_args(args);
	if(parsed_args == NULL)
		return 1;
	if(!next_unnamed_arg(parsed_args, &pcb_name)) {
		printf("Bad usage: PCB name not provided\n");
		sys_free_mem(parsed_args);
		return 1;
	}

	pcb_t *pcb = findPCB(pcb_name);
	if(pcb == NULL) {
		printf("Error: PCB not found\n");
		sys_free_mem(parsed_args);
		return 1;
	}
	sys_free_mem(parsed_args);

	if(pcb->pcb_protection_mode == NOT_DELETABLE) {
		printf("Error: Process %s cannot be deleted\n", pcb->pcb_name);
		return 1;
	} else if(pcb->pcb_protection_mode == DELETABLE_WHEN_SUSPENDED && !(pcb->pcb_process_state == SUSPENDED_READY || pcb->pcb_process_state == SUSPENDED_BLOCKED)) {
		printf("Error: Process %s can only be deleted if suspended first\n", pcb->pcb_name);
		printf("Try running: suspendpcb %s\n", pcb->pcb_name);
		return 1;
	}
	
	removePCB(pcb);
	freePCB(pcb);
	// sys_free_mem(pcb->pcb_stack_bottom);
	// sys_free_mem(pcb);

	return 0;
}

int blockPCB(char *args) {
	char *pcb_name;

	parsed_args *parsed_args = parse_args(args);
	if(parsed_args == NULL)
		return 1;
	if(!next_unnamed_arg(parsed_args, &pcb_name)) {
		printf("Bad usage: PCB name not provided\n");
		sys_free_mem(parsed_args);
		return 1;
	}

	pcb_t *pcb = findPCB(pcb_name);
	if(pcb == NULL) {
		printf("Error: PCB not found\n");
		sys_free_mem(parsed_args);
		return 1;
	}
	sys_free_mem(parsed_args);

	removePCB(pcb);
	switch(pcb->pcb_process_state) {
		case RUNNING:
		case READY:
		case BLOCKED:
			pcb->pcb_process_state = BLOCKED;
			break;
		case SUSPENDED_READY:
		case SUSPENDED_BLOCKED:
			pcb->pcb_process_state = SUSPENDED_BLOCKED;
			break;
		default:
			return 1;
	}
	return insertPCB(pcb);
}

int unblockPCB(char * name) {
	skip_ws(&name);

	// Error handling for name 
	if (name == NULL || strlen(name) > MAX_NAME_SIZE) {
		printf("Error: Name of the PCB is too long\n"); // added error message here instead of printing nothing
		return 1;
	}

	// Verify that PCB exists 
	pcb_t * pcb = findPCB(name);
	
	if (pcb == NULL) {
		print("Error: PCB with that name does not exist\n",34);
		return 1;
	}

	removePCB(pcb);

	// Assign new state and insert into appropriate queue 
	switch(pcb->pcb_process_state) {
		case RUNNING:
		case READY:
		case BLOCKED:
			pcb->pcb_process_state = READY;
			break;
		case SUSPENDED_READY:
		case SUSPENDED_BLOCKED:
		default:
			return 1;
	}

	insertPCB(pcb);
	
	return 0;
}

/********************************************************/
/********************* R4 Stuff Here ********************/
/********************************************************/

int resumeAll(char * p) {
	(void) p;
	pcb_node_t * node = priority_queue->pcbq_head;

	if (node == NULL)  {
		serial_println("Error: Nothing in READY queue");
		return -1;
	}

	while (node != NULL) {
		node->pcb->pcb_process_state = READY;
		node = node->pcbn_next_pcb;
	}

	return 0;
}