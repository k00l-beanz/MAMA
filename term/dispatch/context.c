
#include "context.h"
#include "term/pcb/pcb.h"

int yield(char * p) {
	(void) p;
	asm volatile("int $60");
	return 0;
}

int loadr3(char * args) {
	(void) args; // temp

	// So I'm assuming loadr3 will take in a new process
	// similiar to createPCB. The code that will be used is 
	// below I just need to integrate it with MAMA

	pcb_t * pcb = loadr3CreatePCB(args);
	
	if (pcb == NULL) {
		return 0;
	}

	context * cp = (context *) pcb->pcb_stack_top;
	memset(cp, 0, sizeof(context));
	
	/*
	cp->fs = 0x10;
	cp->gs = 0x10;
	cp->ds = 0x10;
	cp->es = 0x10;
	cp->cs = 0x8;
	cp->ebp = (u32int) pcb->stack_base;
	cp->esp = (u32int) pcb->stack_top;
	cp->eip = (u32int) func;
	cp->eflags = 0x202;
	*/
	// return pcb;

	return 0;
}

/*
	So I copied this from pcb.c because I needed createPCB to return
	a pcb pointer and not an int for loadr3. This will act as the createPCB backend
	command. 

	Another way I thought about doing this was using setupPCB since it does 
	return a pcb pointer and then using insertPCB. createPCB does all the string parsing, 
	error handling, setting up and inserting already though so I thought it was best to just do this.

	lmk if you think of a better way to do this.

	- Maximillian
*/
pcb_t * loadr3CreatePCB(char * args) {
	// The format of createpcb is : pcb_name.pcb_process_class.pcb_priority
	// So like... first_pcb_block.0.3

	char * name;
	char * token;
	int priority, params = 1;
	int process_class;

	skip_ws(&args);
	token = strtok(args,".");
	name = token;

	/** Parse the user input **/
	while (token != NULL) {
		token = strtok(NULL,".");

		switch (params) {
			case 1:
				process_class = atoi(token);
				break;
			case 2:
				priority = atoi(token);
				break;
			default:
				break;
		}

		params++;
	}

	/** Error Handling **/
	/* Check for correct number of parameters */
	if (params == 1) {
		print("Usage: createpcb [NAME.PROCESS_CLASS.PRIORITY]\n",47);
		return NULL;
	}
	else if (params != 4) {
		print("Error: Wrong amount of parameters\n",27);
		return NULL;
	}

	/* Check name for size and whether it exists in a PCB already */
	if (strlen(name) > MAX_NAME_SIZE || name == NULL) {
		print("Error: Name of the PCB is too long\n",35);
		return NULL;
	} 
	else if (findPCB(name) != NULL) {
		print("Error: PCB with that name already exists\n",41);
		return NULL;
	} 

	/* Check whether priority is out of bounds */
	if (priority > MAX_PRIORITY || priority < MIN_PRIORITY) {
		print("Error: Specified priority is out of bounds\n",43);
		return NULL;
	}

	if (process_class != 0 && process_class != 1) {
		printf("Error: Specified process class does not exist\n"); // if the user doesn't enter a numeric value for the process class, this would print garbage data instead of what they entered, so I removed the part of this error message that echos what the user entered
		return NULL;
	}
	
	
	/* Create the PCB */
	pcb_t * pcb = setupPCB(name, process_class, priority);
	
	/* Insert into PCB queue */
	insertPCB(pcb);
	
	return pcb;
}
