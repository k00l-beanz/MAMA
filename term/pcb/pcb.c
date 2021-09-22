
#include "pcb.h"

#include <modules/mpx_supt.h>
#include <include/string.h>
#include <lib/out.h>

/*
	We are going to have two queues for right now.

*/
pcb_queue_t * priority_queue;
pcb_queue_t * fifo_queue;


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
	pcb->pcb_stack_top = pcb->pcb_stack_bottom + MAX_STACK_SIZE;

	/* Zero out memory in the stack frame (SF) */
	memset(pcb, MAX_STACK_SIZE, '\0');

	return pcb;
}

int freePCB(pcb_t * pcb) {
	int free = sys_free_mem(pcb->pcb_stack_bottom);
	free = sys_free_mem(pcb);
	return free;
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
        // TODO Error message here
        return NULL;
    }

    // Iterate through each PCB queues
    // Iterate through selected PCB queue
    // If strcmp(name,pcb->pcb_name)
    // return pcb
    //
    // return NULL

    // temporary
    pcb_t * pcb = allocatePCB();
    return pcb;
    
}

int insertPCB(pcb_t * pcb) {
	if (pcb == NULL) {
		return 1;
	}

	pcb_queue_t *queue;
	switch (pcb->pcb_process_state) {
		case READY:
			queue = priority_queue;
			break;
		case SUSPENDED:
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
		while(node->pcbn_next_pcb != NULL && node->pcbn_next_pcb->pcb->pcb_priority >= pcb->pcb_priority)
			node = node->pcbn_next_pcb;
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
		return 0;
	}
	else if (params != 4) {
		print("Error: Wrong amount of parameters\n",27);
		return 0;
	}

	/* Check name for size and whether it exists in a PCB already */
	if (strlen(name) > MAX_NAME_SIZE || name == NULL) {
		print("Error: Name of the PCB is too long\n",35);
		return 0;
	} 
	/*else if (findPCB(name) != NULL) { TODO: Enable this when findPCB has been written
		print("Error: PCB with that name already exists\n",41);
		return 0;
	} */

	/* Check whether priority is out of bounds */
	if (priority > MAX_PRIORITY || priority < MIN_PRIORITY) {
		print("Error: Specified priority is out of bounds\n",43);
		return 0;
	}

	if (process_class != 0 && process_class != 1) {
		printf("Error: Specified process class %i does not exist\n", process_class);
		return 0;
	}
	

	/* Create the PCB */
	pcb_t *pcb = setupPCB(name, process_class, priority);

	/* Insert into PCB queue */
	insertPCB(pcb);

	return 0;
}
