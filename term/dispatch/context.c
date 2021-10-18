
#include "context.h"
#include "term/pcb/pcb.h"
#include "procsr3.c"

void yield() {
	asm volatile("int $60");
}

int loadr3(char * p) {
	(void) p;
	insertPCB(dispatcher("proc1", &proc1));
	insertPCB(dispatcher("proc2", &proc2));
	insertPCB(dispatcher("proc3", &proc3));
	insertPCB(dispatcher("proc4", &proc4));
	insertPCB(dispatcher("proc5", &proc5));
	return 0;
}

pcb_t * dispatcher(char * name, void (* func) (void)) {
	// pcb with name exists
	if (findPCB(name) == NULL) { 
		return NULL;
	}

	// all processes will be inserted as an application, change where you initialize the pcb
	// Processes are initialized with priority of 4. You will have to change this near initialization of the pcb
	pcb_t * pcb = setupPCB(name, 1, 4);	
	if (pcb == NULL) { // Ensure pcb was setup correctly
		return NULL;
	}
	pcb->pcb_process_state = SUSPENDED_READY;

	context * cp = (context *) pcb->pcb_stack_top;
	memset(cp, 0, sizeof(context));
	
	cp->fs = 0x10;
	cp->gs = 0x10;
	cp->ds = 0x10;
	cp->es = 0x10;
	cp->cs = 0x8;
	cp->ebp = (u32int) pcb->pcb_stack_bottom;
	cp->esp = (u32int) pcb->pcb_stack_top;
	cp->eip = (u32int) func; 
	cp->eflags = 0x202;	

	return pcb;
}