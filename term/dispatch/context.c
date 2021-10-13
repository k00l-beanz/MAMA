
#include "context.h"
#include "term/pcb/pcb.h"

int yield(char * p) {
	(void) p;
	asm volatile("int $60");
	return 0;
}

int loadr3(char * args) {
	(void) args;
	// So I'm assuming loadr3 will take in a new process
	// similiar to createPCB. The code that will be used is 
	// below I just need to integrate it with MAMA

	/*
	pcb_t * pcb = createPCB(args);
	context * cp = (context *) pcb->stack_top;
	memset(cp, 0, sizeof(context));
	cp->fs = 0x10;
	cp->gs = 0x10;
	cp->ds = 0x10;
	cp->es = 0x10;
	cp->cs = 0x8;
	cp->ebp = (u32int) pcb->stack_base;
	cp->esp = (u32int) pcb->stack_top;
	cp->eip = (u32int) func;
	cp->eflags = 0x202;
	return pcb;

	*/
	
	
	return 0;
}