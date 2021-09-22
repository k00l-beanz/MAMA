#include <lib/out.h>
#include <term/args.h>
#include <term/pcb/pcb.h>

int cmd_pcb_suspend(char *args) {
	(void)args;
	return 0;
}

int cmd_pcb_resume(char *args) {
	(void)args;
	return 0;
}

int cmd_pcb_set_priority(char *args) {
	(void)args;
	return 0;
}

int cmd_pcb_show(char *args) {
	char *pcb_name;

	if(!next_unnamed_arg(parse_args(args), &pcb_name)) {
		println("Bad usage: PCB name not provided", 32);
		return 1;
	}

	pcb_t *pcb = findPCB(pcb_name);
	if(pcb == NULL) {
		println("Error: PCB not found", 20);
		return 1;
	}

	printf("%s - ", pcb->pcb_name);
	switch(pcb->pcb_process_state) {
		case READY:
			display_fg_color(GREEN);
			println("READY", 5);
			break;
		case SUSPENDED:
			display_fg_color(YELLOW);
			println("SUSPENDED", 9);
			break;
		// TODO: add colors and text for other states
	}
	display_reset();
	
	printf("   Proc ID: %i\n", 0); // TODO: placeholder until/if process id is added
	printf("     Class: %s\n", pcb->pcb_process_class == 0 ? "SYS_PROCESS" : "APPLICATION");
	printf("  Priority: %i [%s]\n", pcb->pcb_priority, "PLACEHOLDER");

	return 0;
}

int cmd_pcb_show_ready(char *args) {
	(void)args;
	return 0;
}

int cmd_pcb_show_blocked(char *args) {
	(void)args;
	return 0;
}

int cmd_pcb_show_all(char *args) {
	(void)args;
	// TODO: also show the running proc
	cmd_pcb_show_ready(NULL);
	cmd_pcb_show_blocked(NULL);
	return 0;
}
