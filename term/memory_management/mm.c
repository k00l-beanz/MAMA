#include "mm.h"

#include <term/utils.h>
#include <include/core/serial.h>

/// Start address of the heap
u32int start_addr;

mcb_queue_s allocated;
mcb_queue_s free;

/// Allocated Memory Control List
mcb_queue_s * amcb = &allocated;

/// Free Memory Control List
mcb_queue_s * fmcb = &free;

int initHeap(u32int size) {
	int fullHeapSize = size + sizeof(cmcb_s);
	
	// Allocate to the heap
	start_addr = kmalloc(fullHeapSize);
	if (!start_addr) {
		serial_println("Error: Something went wrong during kmalloc");
		return -1;
	}

	// Organize the heap. Both are of type FREE
	// CMCB at the top of the heap w/ all the information
	cmcb_s * head = (cmcb_s *) start_addr;
	head->type = FREE;
	head->addr = start_addr + sizeof(cmcb_s);
	head->size = (u32int) fullHeapSize - sizeof(cmcb_s);
	strcpy(head->name,"Initial FMCB");
	head->next = NULL;
	head->prev = NULL;

	// Initialize free and allocated lists
	fmcb->mcbq_head = head;
	fmcb->mcb_queue_type = FREE;

	amcb->mcbq_head = NULL;
	amcb->mcb_queue_type = ALLOCATED;

	return 0;
}

u32int allocateMemory(u32int size) {
	// Calculate required size for allocated mcb
	u32int required = size;
	u32int ref_size;

	// Is fmcb list empty?
	if (fmcb->mcbq_head == NULL) {
		serial_println("Error: Free Memory Control List is empty.");
		return -1;
	}

	// Iterate through the free memory until a block 
	// with enough space is found
	cmcb_s * queue = fmcb->mcbq_head;
	while (queue != NULL) {
		if (queue->size >= required) {
			ref_size = queue->size;
			break;
		}
		
		queue = queue->next;
	}

	// If no block with enough space is found, throw error
	if (queue == NULL) {
		serial_println("Error: No free memory available");
		return -1;
	}
	
	// Allocate memory
	// 1. Remove mcb that has enough space from the free list
	// 2. Allocate memory / Insert into allocated list
	// 3. Assign free cmcb in the next available free area

	// 1. Remove mcb with enough space
	removeFMCB(queue);
	
	// 2. Allocate memory for the mcb and insert into the AMCB queue
	cmcb_s * newAMCB = queue;
	newAMCB->type = ALLOCATED;
	newAMCB->addr = (u32int) queue->addr;
	newAMCB->size = (u32int) required;
	strcpy(newAMCB->name, "New AMCB\0");
	newAMCB->next = NULL;
	newAMCB->prev = NULL;

	insertAMCB(newAMCB);

	// Update amount of size remaining in fmcb block, if any
	ref_size = ref_size - required;
	if ((ref_size < 1) || (sizeof(cmcb_s) > ref_size)) {
		serial_println("No room to insert new FMCB");
		newAMCB->size = newAMCB->size + ref_size; // avoid unused memory with no cmcb being left over in this case
		return newAMCB->addr;
	}

	// 3. Assign free cmcb in the next available free area and insert into FMCB queue
	cmcb_s * newFMCB = (cmcb_s *) (newAMCB->addr + required);
	
	newFMCB->type = FREE;
	newFMCB->addr = (u32int) newAMCB->addr + required + sizeof(cmcb_s);
	newFMCB->size = (u32int) ref_size - sizeof(cmcb_s);
	strcpy(newFMCB->name, "FMCB Block\0");
	newFMCB->next = NULL;
	newFMCB->prev = NULL;

	insertFMCB(newFMCB);

	return (u32int) newAMCB->addr;
}

void removeFMCB(cmcb_s * cmcb) {

	// Only free cmcb in the list (only head)
	if ((cmcb->prev == NULL) && (cmcb->next == NULL)) {
		fmcb->mcbq_head = NULL;
		return;
	}
	// There is a free cmcb after, but not before (is head w/ members)
	else if ((cmcb->prev == NULL) && (cmcb->next != NULL)) {
		// Assign new head
		fmcb->mcbq_head = cmcb->next;

		// Terminate current head
		fmcb->mcbq_head->prev = NULL;

		return;
	}
	// There is a free cmcb before and after (somewhere in the middle of the list)
	else if ((cmcb->prev != NULL) && (cmcb->next != NULL)) {
		// Save reference to previous cmcb
		cmcb_s * pCmcb = cmcb->prev;

		// Save reference to next cmcb
		cmcb_s * nCmcb = cmcb->next;

		// Link previous and next
		pCmcb->next = nCmcb;
		nCmcb->prev = pCmcb;

		return;
	}
	// There is a previous cmcb but no next free cmcb (tail)
	else if ((cmcb->prev != NULL) && (cmcb->next == NULL)) {
		// Reference previous cmcb
		cmcb_s * pCmcb = cmcb->prev;

		// Terminate the tail
		pCmcb->next = NULL;

		return;
	}

	serial_println("removeFMCB: No condition applies");
	return;
}

void removeAMCB(cmcb_s * cmcb) {

	// Ensure AMCB head exists
	if (amcb->mcbq_head == NULL) {
		serial_println("removeAMCB: No AMCB head exists");
		return;
	}

	// Only head exists
	if ((cmcb->prev == NULL) && (cmcb->next == NULL)) {
		amcb->mcbq_head = NULL;
		return;
	}
	// Head with members
	else if ((cmcb->prev == NULL) && (cmcb->next != NULL)) {
		amcb->mcbq_head = cmcb->next;
		amcb->mcbq_head->prev = NULL;

		return;
	}
	// Somewhere in the middle
	else if ((cmcb->prev != NULL) && (cmcb->next != NULL)) {
		// Save reference to previous cmcb
		cmcb_s * pCmcb = cmcb->prev;

		// Save reference to next cmcb
		cmcb_s * nCmcb = cmcb->next;

		// Link everything up
		pCmcb->next = nCmcb;
		nCmcb->prev = pCmcb;

		return;
	}
	// Removing tail
	else if ((cmcb->prev != NULL) && (cmcb->next == NULL)) {
		// Save reference to previous cmcb
		cmcb_s * pCmcb = cmcb->prev;

		// Terminate current tail
		pCmcb->next = NULL;

		return;
	}
	serial_println("removeAMCB: No condition applies");
	return;
}

void insertAMCB(cmcb_s * mcb) {


	// No allocated mcb's
	if (amcb->mcbq_head == NULL) {
		amcb->mcbq_head = mcb;
		return;
	}
	
	// Organize by increasing address
	cmcb_s * queue = amcb->mcbq_head;
	while (queue != NULL) {

		if (mcb->addr < queue->addr) {

			// Is the newly inserted mcb address lower than the heads address?
			if (mcb->addr < amcb->mcbq_head->addr)
				amcb->mcbq_head = mcb;

			// Reference to previous mcb
			cmcb_s * pCmcb = queue->prev;

			// Link everything up
			pCmcb->next = mcb;
			mcb->prev = pCmcb;
			mcb->next = queue;
			queue->prev = mcb;

			return;
		}
		// New mcb has the highest address insert at the tail
		else if (queue->next == NULL) {
			queue->next = mcb;
			mcb->prev = queue;

			return;
		}

		queue = queue->next;
	}

	serial_println("insertAMCB Error: MCB was not inserted");
	return;
}

void insertFMCB(cmcb_s * mcb) {

	// Is there a head to the fmcb list?
	if (fmcb->mcbq_head == NULL) {
		fmcb->mcbq_head = mcb;
		return;
	}

	// Address in ascending order
	cmcb_s * queue = fmcb->mcbq_head;
	while (queue != NULL) {

		// Found spot to insert either at head or in the middle
		if (mcb->addr < queue->addr) {

			// Is the newly inserted mcb address lower than the heads address?
			if (mcb->addr < fmcb->mcbq_head->addr)
				fmcb->mcbq_head = mcb;

			// Reference to previous node
			cmcb_s * pCmcb = queue->prev;

			// Link everything up
			pCmcb->next = mcb;
			mcb->prev = pCmcb;
			mcb->next = queue;
			queue->prev = mcb;

			return;
		}
		// New mcb has the highest address insert at the tail
		else if (queue->next == NULL) {
			// Create new tail
			queue->next = mcb;
			mcb->prev = queue;

			return;
		}

		queue = queue->next;
	}

	serial_println("insertFMCB Error: MCB was not inserted");
	return;
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

int freeMemory(void * addr) {
	// Does an amcb list even exist?
	if (amcb->mcbq_head == NULL) {
		serial_println("Error: Allocated MCB list is empty");
		return -1;
	}

	u32int intAddr = (u32int) addr;

	// * 1. Set memory of specified address to free
	// * 2. Check below for free block
	// * 	2.a. If one exists, merge
	// * 3. Check above for free block
	// * 	3.a If one exists, merge

	// 1. Set memory of specified address to free
	// Ensure address exists
	cmcb_s * queue = amcb->mcbq_head;
	while (queue != NULL) {
		if (queue->addr == intAddr) break;

		queue = queue->next;
	}

	// MCB with address was not found
	if (queue == NULL) {
		serial_println("Error: No AMCB with specified address exists");
		return -1;
	}
	
	removeAMCB(queue);

	// Assign space as free and insert into fmcb list
	queue->type = FREE;
	strcpy(queue->name, "New Free Block");

	insertFMCB(queue);


	// 2. Check below for free block
	// 		2.a If one exists, merge
	cmcb_s * below = (cmcb_s *)(queue->addr + queue->size);
	if (below->type == FREE) {

		// Inherit qualities of below with newly created fmcb
		queue->size = queue->size + below->size + sizeof(cmcb_s);
		strcpy(queue->name, below->name);

		// Terminate below
		removeFMCB(below);
	}

	// 3. Check above for free block
	// 		3.a If one exists, merge
	cmcb_s * above = fmcb->mcbq_head;
	while (above != NULL) {
		
		if ((above->addr + above->size + sizeof(cmcb_s)) == queue->addr) {

			// Above inherits the qualities of current mcb
			above->size = (u32int) above->size + queue->size + sizeof(cmcb_s);
			strcpy(above->name, queue->name);
			
			// Terminate current mcb
			removeFMCB(queue);

			// Can only have one thing above
			break;
		}
		above = above->next;
	}

	return 0;
}

int isEmpty(char * p) {
	(void) p;
	if(amcb->mcbq_head == NULL){
		serial_println("Memory is empty");
		return 1;
	}
	else {
		serial_println("Memory is not empty");
		return 0;
	}	
} 

int showFree(char * p) {
	(void) p;
	
	cmcb_s * free = fmcb->mcbq_head;
	if(fmcb == NULL) {
		printf("No free memory found\n");
		return 0;
	}
	while(free != NULL) {
		printf("Block %s - ", free->name);
		if(free->type == ALLOCATED)
			display_fg_color(RED);
		else
			display_fg_color(GREEN);
		printf(free->type == ALLOCATED ? "ALLOCATED" : "FREE");
		display_reset();
		printf(" - base addr: %i, size: %i bytes\n", free->addr, free->size);
		free = free->next;
	}

	return 0;
}
