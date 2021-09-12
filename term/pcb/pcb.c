
#include "pcb.h"
#include "mpx_supt.h"

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


	/**** Set values of PCB ****/
	/* Determine if a PCB with the same name exists */
	// TODO I'll need searchPCB in order to add this error handling
	// PCB name can only be 32 bytes long too
	pcb->pcb_name = name;
	pcb->pcb__process_class = process_class;

	/* Can only have a priority level from 0 to 9 */
	if (priority > MAX_PRIORITY || priority < MIN_PRIORITY) {
		/// TODO print error message 
		return NULL;
	}
	pcb->pcb_priority = priority;
	pcb->pcb_process_state = READY;

	/* Allocate memory */
	pcb_t *pcb = allocatePCB();

	if (pcb == NULL) {
		return NULL;
	}

	return pcb;
}

pcb_t * findPCB(char * name) {
	/* Check for valid name */
	if (strlen(name) > 32 || name == NULL) {
		// TODO Error message here
		return NULL;
	}

	
}

