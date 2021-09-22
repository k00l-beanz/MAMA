
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

	return pcb;
}

int freePCB(pcb_t * pcb) {
	int free = 1;
	free = sys_free_mem(pcb->pcb_stack_bottom);
	free = sys_free_mem(pcb);
	return free;
}

void insertPCB(pcb_t *pcb) {
	
}
