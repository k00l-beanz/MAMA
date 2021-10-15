#ifndef CONTEXT_H
#define CONTEXT_H

#include "term/pcb/pcb.h"

/// Context of the currently operating process
typedef struct Context {
	/// Segment registers
	u32int gs, fs, es, ds;

	/// General purpose registers
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;

	// Other special registers
	u32int eip, cs, eflags;
} context;

/**
 * Causes commhand to yield
 * 
 * Forces commhand to yield to other processes.
 * If any processes are in the ready queue, they will
 * be executed.
 * 
 * @param p Empty parameter
 * 
 * @return Returns 0 upon success
 * 
*/
int yield(char * p);

/**
 * Loads r3 'processes'
 * 
 * Loads all r3 'processes' into memory in a suspended ready
 * state at any priority of the users choosing
 * 
 * @param p Empty parameter
 * 
 * @return Returns 0 upon success, 1 upon error **This may change
 * 
*/
int loadr3(char * p);

/**
 * CreatePCB for loadr3
 * 
 * CreatePCB function for r3. This method behaves the
 * same way as 'createPCB' in pcb.c with the execption
 * that this method returns a pointer to the newly created
 * pcb.
 * 
 * @param args PCB to be creates and used as the currentlly operating process
 * @return Upon sucess, pointer to PCB, otherwise returns NULL 
*/
pcb_t * loadr3CreatePCB(char * args);

void loadr3BackEnd(char * pcb, void (*func) (void)); 

#endif
