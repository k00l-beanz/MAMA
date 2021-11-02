#include "mm.h"
#include "term/utils.h"
#include <include/core/serial.h>

/// Start address of the heap
u32int start_addr;

mcb_queue_s allocated;
mcb_queue_s free;

/// Allocated Memory Control List
mcb_queue_s * amcb = &allocated;

/// Free Memory Control List
mcb_queue_s * fmcb = &free;

/// Full heap size
int fullHeapSize;

int initHeap(char * p) {
	skip_ws(&p);
	// Syntax for the cmd is: initheap [size]
	serial_println("");
	serial_println("DEBUG: initHeap");
	// Convert parameter from char * to int
	int size = atoi(p);
	int fullHeapSize = size + sizeof(cmcb_s);
	
	// Allocate to the heap
	start_addr = kmalloc(fullHeapSize);

	// Organize the heap. Both are of type FREE
	// ** CMCB at the top of the heap 		**
	// ** LMCB at the bottom of the heap 	**

	// CMCB at the top of the heap w/ all the information
	cmcb_s * head = (cmcb_s *) start_addr;
	head->type = FREE;
	head->addr = start_addr + sizeof(cmcb_s);
	head->size = (u32int) fullHeapSize - sizeof(cmcb_s);
	strcpy(head->name,"Initial CMCB");
	head->next = NULL;
	head->prev = NULL;

	printf("Heads Address: %i\n",(int) head->addr);
	printf("Heads Size: %i\n", (int) head->size);

	// Initialize free and allocated lists
	fmcb->mcbq_head = head;
	fmcb->mcb_queue_type = FREE;

	amcb->mcbq_head = NULL;
	amcb->mcb_queue_type = ALLOCATED;

	serial_println("");
	return 0;
}

int allocateMemory(char * size) {
	skip_ws(&size);
	serial_println("");
	serial_println("DEBUG: allocateMemory");
	// Calculate required size for allocated mcb
	u32int required = (u32int) (atoi(size) + sizeof(cmcb_s));
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

	printf("Available Memory: %i\n",queue->size);
	printf("Free Queues Head Address: %i\n", queue->addr);
	printf("Allocated Queues Head Address: %i\n", amcb->mcbq_head->addr);

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
	
	// 2. Allocate memory for the mcb
	cmcb_s * newAMCB = queue;
	newAMCB->type = ALLOCATED;
	newAMCB->addr = (u32int) queue->addr;
	newAMCB->size = (u32int) required;
	strcpy(newAMCB->name, "New AMCB\0");
	newAMCB->next = NULL;
	newAMCB->prev = NULL;

	printf("New AMCB addr: %i\n", newAMCB->addr);
	printf("New AMCB Size: %i\n", newAMCB->size);

	insertAMCB(newAMCB);

	printf("AMCB addr after insert: %i\n", newAMCB->addr);
	printf("AMCB size after insert: %i\n", newAMCB->size);

	serial_println("");

	// 3. Assign free cmcb in the next available free area
	cmcb_s * newFMCB = (cmcb_s *) newAMCB + required;
	newFMCB->type = FREE;
	newFMCB->addr = (u32int) newAMCB->addr + required + sizeof(cmcb_s);
	newFMCB->size = (u32int) ref_size - (required + sizeof(cmcb_s));
	newFMCB->next = NULL;
	newFMCB->prev = NULL;

	// Make sure there is enough memory remaining to insert
	// a new FMCB
	if (newFMCB->size > ref_size) {
		serial_println("Error: Not enough memory to insert new fmcb");
		return -1;
	}

	printf("New FMCB addr: %i\n", newFMCB->addr);
	printf("New FMCB size: %i\n", newFMCB->size);
	serial_println("");
	insertFMCB(newFMCB);
	printf("FMCB addr after insert: %i\n", newFMCB->addr);
	printf("FMCB size after insert: %i\n", newFMCB->size);

	serial_println("");

	printf("FMCB head addr: %i\n", fmcb->mcbq_head->addr);
	printf("AMCB head addr: %i\n", amcb->mcbq_head->addr);

	serial_println("");

	printf("FMCB remaining size: %i\n", fmcb->mcbq_head->size);

	serial_println("");

	return 0;
}

void removeFMCB(cmcb_s * cmcb) {
	serial_println("");
	serial_println("DEBUG: removeFMCB");

	// Only free cmcb in the list (only head)
	if ((cmcb->prev == NULL) && (cmcb->next == NULL)) {
		serial_println("Removing only head");
		fmcb->mcbq_head = NULL;

		serial_println("");
		return;
	}
	// There is a free cmcb after, but not before (is head w/ members)
	else if ((cmcb->prev == NULL) && (cmcb->next != NULL)) {
		serial_println("Head with members");
		// Assign new head
		fmcb->mcbq_head = cmcb->next;

		// Terminate current head
		fmcb->mcbq_head->prev = NULL;

		serial_println("");
		return;
	}
	// There is a free cmcb before and after (somewhere in the middle of the list)
	else if ((cmcb->prev != NULL) && (cmcb->next != NULL)) {
		serial_println("Somewhere in the middle");
		// Save reference to previous cmcb
		cmcb_s * pCmcb = cmcb->prev;

		// Save reference to next cmcb
		cmcb_s * nCmcb = cmcb->next;

		// Link previous and next
		pCmcb->next = nCmcb;
		nCmcb->prev = pCmcb;

		serial_println("");
		return;
	}
	// There is a previous cmcb but no next free cmcb (tail)
	else if ((cmcb->prev != NULL) && (cmcb->next == NULL)) {
		serial_println("At the tail");
		// Reference previous cmcb
		cmcb_s * pCmcb = cmcb->prev;

		// Terminate the tail
		pCmcb->next = NULL;

		serial_println("");
		return;
	}

	serial_println("");
}

void insertAMCB(cmcb_s * mcb) {
	serial_println("");
	serial_println("DEBUG: insertAMCB");

	printf("Addr of mcb being inserted: %i\n", mcb->addr);

	// No allocated mcb's
	if (amcb->mcbq_head == NULL) {
		serial_println("AMCB is empty. Creating head");
		amcb->mcbq_head = mcb;

		serial_println("");
		return;
	}
	
	// Organize by increasing address
	cmcb_s * queue = amcb->mcbq_head;
	while (queue->next != NULL) {

		if (mcb->addr < queue->addr) {
			// Reference to previous mcb
			cmcb_s * pCmcb = queue->prev;

			// Link everything up
			pCmcb->next = mcb;
			mcb->prev = pCmcb;
			mcb->next = queue;
			queue->prev = mcb;

			return;
		}
		
		printf("Iterating through amcb list, curr addr: %i\n", queue->addr);
		queue = queue->next;
	}

	// New mcb has the highest address,
	// Inserting at the tail
	queue->next = mcb;
	mcb->prev = queue;


	serial_println("");
}

void insertFMCB(cmcb_s * mcb) {
	serial_println("");
	serial_println("DEBUG: insertFMCB");

	// Is there a head to the fmcb list?
	if (fmcb->mcbq_head == NULL) {
		serial_println("Create new free head");
		fmcb->mcbq_head = mcb;
		// serial_println(fmcb->mcbq_head->name);

		serial_println("");
		return;
	}

	// Address in ascending order
	cmcb_s * queue = fmcb->mcbq_head;
	while (queue->next != NULL) {

		if (mcb->addr < queue->addr) {
			// Reference to previous node
			cmcb_s * pCmcb = queue->prev;

			// Link everything up
			pCmcb->next = mcb;
			mcb->prev = pCmcb;
			mcb->next = queue;
			queue->prev = mcb;

			return;
		}
		printf("Iterating through fmcb list, curr addr: %i\n", queue->addr);
		queue = queue->next;
	}

	// New mcb has the highest address
	// Insert at the tail
	queue->next = mcb;
	mcb->prev = queue;

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

int isEmpty() {
	if(amcb->mcbq_head == NULL){
		serial_println("Memory is empty");
		return 0;
	}
	else {
		serial_println("Memory is not empty");
		return -1;
	}	
} 
