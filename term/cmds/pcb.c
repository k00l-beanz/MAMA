/*
#include <lib/out.h>
#include <term/args.h>
#include <term/pcb/pcb.h>

extern pcb_queue_t * priority_queue;
extern pcb_queue_t * fifo_queue;

int cmd_pcb_set_priority(char *args) {
	(void)args;
	return 0;
}


int cmd_pcb_show(char *args) {
	char *pcb_name;

	parsed_args *parsed_args = parse_args(args);
	if(!next_unnamed_arg(parsed_args, &pcb_name)) {
		println("Bad usage: PCB name not provided", 32);
		sys_free_mem(parsed_args);
		return 1;
	}
	sys_free_mem(parsed_args);

	pcb_t *pcb = findPCB(pcb_name);
	if(pcb == NULL) {
		println("Error: PCB not found", 20);
		return 1;
	}

	printf("Process %s - ", pcb->pcb_name);
	switch(pcb->pcb_process_state) {
		case RUNNING:
			display_fg_color(GREEN);
			printf("RUNNING\n");
			display_reset();
			break;
		case READY:
			display_fg_color(GREEN);
			printf("READY");
			display_reset();
			printf(" (not suspended)\n");
			break;
		case BLOCKED:
			display_fg_color(YELLOW);
			printf("BLOCKED");
			display_reset();
			printf(" (not suspended)\n");
			break;
		case SUSPENDED_READY:
			display_fg_color(RED);
			printf("READY");
			display_reset();
			printf(" (");
			display_fg_color(RED);
			printf("SUSPENDED");
			display_reset();
			printf(")\n");
			break;
		case SUSPENDED_BLOCKED:
			display_fg_color(RED);
			printf("BLOCKED");
			display_reset();
			printf(" (");
			display_fg_color(RED);
			printf("SUSPENDED");
			display_reset();
			printf(")\n");
			break;
	}
	
	printf("   Proc ID: %i\n", 0); // TODO: placeholder until/if process id is added
	printf("     Class: %s\n", pcb->pcb_process_class == 0 ? "SYS_PROCESS" : "APPLICATION");
	char *priority_str;
	if(pcb->pcb_priority < 3)
		priority_str = "LOW";
	else if(pcb->pcb_priority < 6)
		priority_str = "MEDIUM";
	else if(pcb->pcb_priority < 8)
		priority_str = "HIGH";
	else if(pcb->pcb_priority < 9)
		priority_str = "CRITICAL";
	else
		priority_str = "MAXIMUM";
	printf("  Priority: %i [%s]\n", pcb->pcb_priority, priority_str);

	return 0;
}

int cmd_pcb_show_ready(char *args) {
	(void)args;
	return 0;
}

int cmd_pcb_show_blocked(char *args) {
	(void)args;

	pcb_node_t *node = fifo_queue->pcbq_head;
	if(node == NULL) {
		printf("No blocked PCBs found");
		return 0;
	}
	while(node != NULL) {
		cmd_pcb_show(node->pcb->pcb_name);
		printf("\n");
		node = node->pcbn_next_pcb;
	}
	return 0;
}

int cmd_pcb_show_all(char *args) {
	(void)args;
	// TODO: also show the running proc
	cmd_pcb_show_ready(NULL);
	cmd_pcb_show_blocked(NULL);
	return 0;
}

int cmd_pcb_suspend(char *args) {
	char *pcb_name;

	parsed_args *parsed_args = parse_args(args);
	if(!next_unnamed_arg(parsed_args, &pcb_name)) {
		printf("Bad usage: PCB name not provided\n");
		sys_free_mem(parsed_args);
		return 1;
	}
	sys_free_mem(parsed_args);

	pcb_t *pcb = findPCB(pcb_name);
	if(pcb == NULL) {
		printf("Error: PCB not found\n");
		return 1;
	}

	removePCB(pcb);
	switch(pcb->pcb_process_state) {
		case RUNNING:
		case READY:
		case SUSPENDED_READY:
			pcb->pcb_process_state = SUSPENDED_READY;
			break;
		case BLOCKED:
		case SUSPENDED_BLOCKED:
			pcb->pcb_process_state = SUSPENDED_BLOCKED;
			break;
		default:
			return 1;
	}
	return insertPCB(pcb);
}

int cmd_pcb_resume(char *args) {
	char *pcb_name;

	parsed_args *parsed_args = parse_args(args);
	if(!next_unnamed_arg(parsed_args, &pcb_name)) {
		printf("Bad usage: PCB name not provided\n");
		sys_free_mem(parsed_args);
		return 1;
	}
	sys_free_mem(parsed_args);

	pcb_t *pcb = findPCB(pcb_name);
	if(pcb == NULL) {
		printf("Error: PCB not found\n");
		return 1;
	}

	removePCB(pcb);
	switch(pcb->pcb_process_state) {
		case SUSPENDED_READY:
			pcb->pcb_process_state = READY;
			break;
		case SUSPENDED_BLOCKED:
			pcb->pcb_process_state = BLOCKED;
			break;
		default:
			return 0; // do nothing if PCB isn't suspended
	}
	return insertPCB(pcb);
}

int cmd_pcb_delete(char *args) {
	char *pcb_name;

	parsed_args *parsed_args = parse_args(args);
	if(!next_unnamed_arg(parsed_args, &pcb_name)) {
		printf("Bad usage: PCB name not provided\n");
		sys_free_mem(parsed_args);
		return 1;
	}
	sys_free_mem(parsed_args);

	pcb_t *pcb = findPCB(pcb_name);
	if(pcb == NULL) {
		printf("Error: PCB not found\n");
		return 1;
	}

	removePCB(pcb);
	sys_free_mem(pcb->pcb_stack_bottom);
	sys_free_mem(pcb);

	return 0;
}

int cmd_pcb_block(char *args) {
	char *pcb_name;

	parsed_args *parsed_args = parse_args(args);
	if(!next_unnamed_arg(parsed_args, &pcb_name)) {
		printf("Bad usage: PCB name not provided\n");
		sys_free_mem(parsed_args);
		return 1;
	}
	sys_free_mem(parsed_args);

	pcb_t *pcb = findPCB(pcb_name);
	if(pcb == NULL) {
		printf("Error: PCB not found\n");
		return 1;
	}

	removePCB(pcb);
	switch(pcb->pcb_process_state) {
		case RUNNING:
		case READY:
		case BLOCKED:
			pcb->pcb_process_state = BLOCKED;
			break;
		case SUSPENDED_READY:
		case SUSPENDED_BLOCKED:
			pcb->pcb_process_state = SUSPENDED_BLOCKED;
			break;
		default:
			return 1;
	}
	return insertPCB(pcb);
}

*/