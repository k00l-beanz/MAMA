#include "mm.h"
#include <include/core/serial.h>

#include <lib/out.h>

/// Start address of the heap
u32int start_addr;

/// Allocated Memory Control List
mcb_queue_s * amcb;

/// Free Memory Control List
mcb_queue_s * fmcb;


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

	// CMCB at the top of the heap w/ all the information
	cmcb_s * head = (cmcb_s *) start_addr;
	head->type = FREE;
	head->addr = start_addr + sizeof(cmcb_s);
	head->size = (u32int) size;
	strcpy(head->name,"Initial CMCB");
	head->next = NULL;
	head->prev = NULL;

	// LMCB at the at the bottom.
	lmcb_s * bottom = (lmcb_s *) ((start_addr + fullHeapSize) - sizeof(lmcb_s)); // I think this math/logic is right. Correct it if its wrong
	bottom->type = FREE;
	bottom->size = (u32int) size;

	// Initialize free and allocated lists
	fmcb->mcbq_head = head;
	fmcb->mcbq_tail = head;
	fmcb->mcb_count = 1;
	fmcb->mcb_queue_type = FREE;

	amcb->mcbq_head = NULL;
	amcb->mcbq_tail = NULL;
	amcb->mcb_count = 0;
	amcb->mcb_queue_type = ALLOCATED;

	return 0;
}

int showAllocated(char *discard) {
	(void)discard;
	
	cmcb_s *block = amcb->mcbq_head;
	if(block == NULL) {
		printf("No allocated memory found\n");
		return 0;
	}
	while(block != NULL) {
		printf("Block %s - ", block->name);
		if(block->type == ALLOCATED)
			display_fg_color(RED);
		else
			display_fg_color(GREEN);
		printf(block->type == ALLOCATED ? "ALLOCATED" : "FREE");
		display_reset();
		printf(" - base addr: %i, size: %i bytes\n", block->addr, block->size);
		block = block->next;
	}
	return 0;
}