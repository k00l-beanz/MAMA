
#include "pcb.h"

#include <modules/mpx_supt.h>
#include <include/string.h>
#include <lib/out.h>

pcb_queue_t *pcb_queues[4];


/********************************************************/
/****************** Backend stuff here ******************/
/********************************************************/

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

pcb_t * setupPCB(char * name, pc_t process_class, int priority) {

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
	if (process_class != SYS_PROCESS || process_class != APPLICATION) {
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

	if (pcb == NULL) {
		return;
	}

	

	return;
}

/********************************************************/
/*************** User Command stuff here ****************/
/********************************************************/

// void createPCB(char * argv[], int argc)

void createpcb() {
	return;
}
void createPCB(char * name, pc_t process_class, int priority) {

	/* Check name for size and whether it exists in a PCB already */
	if (strlen(name) > MAX_NAME_SIZE || name == NULL) {
		// Print error message here
		return;
	} 
	else if (findPCB(name) != NULL) {
		// Error message here
		return;
	}

	/* Check whether priority is out of bounds */
	if (priority > MAX_PRIORITY || priority < MIN_PRIORITY) {
		// Error message here
		return;
	}

	/* Check whether this is a valid process class */
	if (process_class != SYS_PROCESS || process_class != SYS_PROCESS) {
		// Error message here
		return;
	}

	//pcb_t * pcb = setupPCB(name, process_class, priority);

	// Insert into appropriate queue

	// Successful message here
	return;
}

