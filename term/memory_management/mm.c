#include "mm.h"
#include <include/core/serial.h>

/// Start address of the heap
u32int start_addr;

int initHeap(char * p) {
	// Syntax for the cmd is initheap [size]

	// Convert parameter from char to int
	int size = atoi(p);
	int fullHeapSize = size + sizeof(cmcb_s) + sizeof(lmcb_s);
	
	// Allocate to the heap
	start_addr = kmalloc(fullHeapSize);

	// Organize the heap. Both are of type FREE
	// ** CMCB at the top of the heap 		**
	// ** LMCB at the bottom of the heap 	**

	// CMCB at the top of the heap
	cmcb_s * head = (cmcb_s *) start_addr;
	head->type = FREE;
	head->next = NULL;
	head->prev = NULL;


	// LMCB at the at the bottom.
	lmcb_s * bottom = (lmcb_s *) ((start_addr + fullHeapSize) - sizeof(lmcb_s));
	bottom->type = FREE;


	(void) head;
	(void) bottom;

	// Initialize free and allocated lists

	(void) start_addr;
	return 0;
}