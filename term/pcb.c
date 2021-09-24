
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
	if (pcb->pcb_stack_top == NULL) {
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

	/* PCB name can only be 32 bytes long too */
	if (strlen(name) > MAX_NAME_SIZE) {
		print("PCB name too long. Please use a shorter name\n",45);
		return NULL;
	} else if (findPCB(name) != NULL) { /* PCB name cannot already exist */
		print("PCB name already in use. Please use a different name\n",53);
		return NULL;
	}

	/* Can only have a priority level from 0 to 9 */
	if (priority > MAX_PRIORITY || priority < MIN_PRIORITY) {
		print("PCB priority out of range\n",26);
		return NULL;
	}

	/* Ensure process class is SYS_PROCESS or APPLICATION */
	if (process_class != 0 || process_class != 1) {
		print("Unknown process class\n",22);
		return NULL;
	}

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

void insertPCB(pcb_t * pcb) {
	/* Got carried away when writing createPCB and started writing this.
		Started it for you Austin */
	if (pcb == NULL) {
		return;
	}

	/*
	switch (pcb->pcb_process_state) {
		case READY:
			break;
		case SUSPENDED:
			break;
		default:
			break;
	}
	*/
	return;
}

/********************************************************/
/*************** User Command stuff here ****************/
/********************************************************/


/*showAll this user command will display all PCB in three queues*/
int showAll(){
showReady();
showBlocked();
return 0;
}

int setPriority(char name[30], int newPriority){
	//creating a local pcb pointer
	PCB *willDelete = findPCB(name);

	//checking if the PCB is found
	if(*willDelete == NULL){
		return 0;
	} else if(willDelete ->pcb_process_class == 0){
		print("Can't edit system process\n", 26);
		return 0;
	}else{
		//deleting the old PCB status
		if(willDelete->READY == 1 && willDelete-> suspended == 0){
			removePCB(willDelete, &rea)
		}
	}
}
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
	/*else if (findPCB(name) != NULL) { Enable this when findPCB has been written
		print("Error: PCB with that name already exists\n",41);
		return 0;
	} */

	/* Check whether priority is out of bounds */
	if (priority > MAX_PRIORITY || priority < MIN_PRIORITY) {
		print("Error: Specified priority is out of bounds\n",43);
		return 0;
	}


	/* Check whether this is a valid process class */
	/*	I have no idea why this doesn't work so I removed it temporarily for debugging.
	if (process_class != 0 || process_class != 1) {
		print("Error: Specified process class does not exist\n",46);
		return 0;
	}
	*/
	

	/* Create the PCB */
	pcb_t *pcb = allocatePCB(name, process_class, priority);

	/* Insert into PCB queue */
	insertPCB(pcb);

	return 0;
}