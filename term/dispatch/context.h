#ifndef CONTEXT_H
#define CONTEXT_H

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

#endif
