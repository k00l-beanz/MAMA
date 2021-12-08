#include <include/string.h>
#include <modules/mpx_supt.h>
#include "visuals/colorize.c"
#include "visuals/clear.c"
#include "history.c"
#include "commhand.h"
#include "commands.h"
#include "visuals/syntax_highlight.h"
#include "visuals/hints.h"
#include <lib/out.c>
#include "dnt/dnt.c"
#include "utils.h"
#include "ascii/mama.c"
#include "dispatch/context.c"
#include "pcb/pcb.c"
#include "memory_management/mm.c"
#include <term/args.h>

typedef int (*cmd_func_t)(char *);

typedef struct cmd_mapping {
    char *cmd_name;
    cmd_func_t cmd_handler;
    char *default_args;
} cmd_mapping;

int cmd_alias(char *);

/* mapping of command names to their associated functions
 * term/commhand.c uses this to find what function should be ran for a given command
 * eventually, this could also provide other information, such as a function to run to print help text for each command
 * I thought this would be cleaner than a bunch of hardcoded if(cmd == "help") ... else if(cmd == "shutdown") ... statements
 * or a massive switch statement with all the command names and functions hardcoded in
 *
 * For new commands, add another struct with the command name as a string and a pointer to its handler function, like below
 * The function to handle the command has to follow the signature for cmd_func_t, which is basically the following
 * int my_handler_func(char *arg_str) { ... }
 */
cmd_mapping cmd_mappings[MAX_CMD_COUNT] = {
	{
		"help",
		&cmd_help,
		""
	},
	{
		"shutdown",
		&cmd_shutdown,
		""
	},
	{
		"version",
		&cmd_version,
		""
	},
	{
		"gettime",
		&gettime,
		""
	},
	{
		"settime",
		&settime,
		""
	},
	{
		"getdate",
		&getdate,
		""
	},
	{
		"setdate",
		&setdate,
		""
	},
	{
		"echo",
		&cmd_echo,
		""
	},
	{
		"showpcb",
		&showPCB,
		""
	},
	{
		"showallpcb",
		&showAll,
		""
	},
	{
		"showreadypcb",
		&showReady,
		""
	},
	{
		"showblockedpcb",
		&showBlocked,
		""
	},
	{
		"setprioritypcb",
		&setPriority,
		""
	},
	{
		"resumepcb",
		&resumePCB,
		""
	},
	{
		"suspendpcb",
		&suspendPCB,
		""
	},
	{
		"resumeallpcb",
		&resumeAll,
		""
	},
	{
		"loadr3",
		&loadr3,
		""
	},
	{
		"setalarm",
		&setAlarm,
		""
	},
	{
		"showalarms",
		&showAlarms,
		""
	},
	{
		"freealarm",
		&freeAlarm,
		""
	},
	{
		"showalloc",
		&showAllocated,
		""
	},
	{
		"showfree",
		&showFree,
		""
	},
	{
		"isempty",
		&isEmpty,
		""
	},
	{
		"arg-test",
		&cmd_argtest,
		""
	},
	{
		"clear",
		&cmd_clear,
		""
	},
	{
		"alias",
		&cmd_alias,
		""
	},
	{
		"r6",
		&cmd_test_r6,
		""
	}
};


int is_name_char(char);
void extract_cmd_name(char *, char *, int *, int *);
cmd_mapping *fetch_cmd_mapping(char *);

extern pcb_queue_t *priority_queue;

/**
 * Displays command line and interprets inputted commands
 * 
 * Parses through the input that was polled from the command line
 * and interprets the command that was inputted (typically the first word)
 * 
 * @return Returns 0 upon success, -1 upon error
*/
void commhand() {
	mama();

	cmd_test_r6(NULL);

	syntax_init();

	char cmd_name[MAX_CMD_NAME_LEN + 1];
	int cmd_name_len = 0;

	int running = 1;

	/* Initialize PCB */
	//initPCB();
	
	/* Driver loop */
	while(running) {
		/* Prints prompt */
		display_fg_color(GREEN);
		printf("~--> ");
continue;
		display_reset();

		/* Reads in user input */
		char *cmd_str = hist_next_frame();
		syntax_enable_highlighting();
		read(cmd_str, MAX_CMD_STRING_LEN);
		println("", 0);
		display_reset();
		syntax_disable_highlighting();

		/* Extract command name (typically first word) from command string */
		int args_start_index;
		extract_cmd_name(cmd_str, cmd_name, &cmd_name_len, &args_start_index);

		/* 
		 * This if-stmt prevents the "Unrecognized command: " block below when the user
		 * just hits return. Hope this doesn't break anything....
		 * 
		 * Sorry Austin I couldn't figure out how to integrate this into your if-else block 
		 * below so I did this in kind of a trivial way. You are more than welcome to implement
		 * this better if you'd like.
		 *
		 * I modified this slightly to make a distinction between just hitting return and entering a bad
		 * command name
		*/
		if (cmd_name[0] == '\0') {
			if(cmd_str[args_start_index] == '\0') {
				/* Prevents the "Unrecognized command: " block below when the user just hits enter 
				 * or enters only whitespace.
				 * Don't add these entries to the command history.
				*/
				hist_discard_last_frame();
				continue;
			} else {
				/* Prints a prompt if the user entered something that doesn't contain a command name
				 * but also isn't just whitespace. An example would be inputting "."
				*/
				println("Bad syntax. Command string must start with a command name", 57);
				continue;
			}
		}

		cmd_mapping *mapping = fetch_cmd_mapping(cmd_name);

		int cmd_exit_code;
		if(mapping != NULL) {
			int default_args_len = mapping->default_args != NULL ? strlen(mapping->default_args) : 0;
			char args[default_args_len + strlen(cmd_str + args_start_index) + 1];
			/* command handlers get a copy of the arguments string so that they are
			 * free to modify it without affecting the command history
			*/
			if(default_args_len > 0)
				strcpy(args, mapping->default_args); // note - this means argument strings passed to functions might not start with a space, although some expect this
			strcpy(args + default_args_len, cmd_str + args_start_index);
			cmd_exit_code = (mapping->cmd_handler)(args);
		} else {
			print("Unrecognized command: ", 22);
			println(cmd_name, cmd_name_len);
		}

		/* Command shutdown kills driver loop */
		if(strcmp(cmd_name, "shutdown") == 0 && cmd_exit_code == 0) {
			running = 0;
			priority_queue->pcbq_head = NULL;
			sys_req(EXIT,DEFAULT_DEVICE,NULL,NULL);
		}
		
		sys_req(IDLE,DEFAULT_DEVICE,NULL,NULL);
	}
}


/*
 * Procedure: fetch_cmd_handler
 * Description: Returns the associated command handler (as a function pointer) for the specified command
 */
cmd_mapping *fetch_cmd_mapping(char *cmd_name) {	
	int i = 0;
	while(cmd_mappings[i].cmd_name != NULL) {
		if(strcmp(cmd_name, cmd_mappings[i].cmd_name) == 0)
			return &cmd_mappings[i];
		i++;
	}
	return NULL;
}

/*
 * Procedure: extract_cmd_name
 * Description: Extracts the command (typically the first word) from the command
 *  string.
*/
void extract_cmd_name(char *cmd_str, char *cmd_name, int *cmd_name_len, int *args_start_index) {
	int ws_skip_len = 0;
	while(isspace(cmd_str + ws_skip_len))
		ws_skip_len++;
	*cmd_name_len = 0;
    int i;
	for(i = 0; i < MAX_CMD_NAME_LEN && is_name_char(cmd_str[ws_skip_len + i]); i++) {
        *cmd_name_len += 1;
		cmd_name[i] = cmd_str[ws_skip_len + i];
	}

	cmd_name[*cmd_name_len] = '\0';
	*args_start_index = *cmd_name_len + ws_skip_len;
}

int cmd_alias(char *arg_str) {
	parsed_args *args = parse_args(arg_str);
	if(args == NULL) {
		return 1;
	}
	if(args->unnamed_arg_count != 2) {
		printf("Error: bad usage\n");
		printf("Try: alias new_cmd_name alias_target [ -a \"optional arguments string\" ]\n");
		sys_free_mem(args);
		return 1;
	}
	if(strlen(args->unnamed_args[0]) > MAX_CMD_NAME_LEN || strlen(args->unnamed_args[1]) > MAX_CMD_NAME_LEN) {
		printf("Error: command names longer than %i characters are not supported\n", MAX_CMD_NAME_LEN);
		sys_free_mem(args);
		return 1;
	}

	int cmd_count = 0;
	while(cmd_count < MAX_CMD_COUNT && cmd_mappings[cmd_count].cmd_name != NULL)
		cmd_count++;

	if(cmd_count >= MAX_CMD_COUNT) {
		printf("Error: maximum number of aliases reached\n");
		sys_free_mem(args);
		return 1;
	}

	cmd_mapping *alias_target_mapping = fetch_cmd_mapping(args->unnamed_args[1]);
	if(alias_target_mapping == NULL) {
		printf("Error: command %s not found\n", args->unnamed_args[1]);
		sys_free_mem(args);
		return 1;
	}

	if(fetch_cmd_mapping(args->unnamed_args[0]) != NULL) {
		printf("Error: command %s already exists; you cannot create another alias with this name\n", args->unnamed_args[0]);
		sys_free_mem(args);
		return 1;
	}

	char *default_args = NULL;
	named_arg(args, "a", &default_args);
	char *cmd_name = (char *)sys_alloc_mem(strlen(args->unnamed_args[0]) + 1);
	strcpy(cmd_name, args->unnamed_args[0]);

	cmd_mappings[cmd_count] = (cmd_mapping){
		cmd_name,
		alias_target_mapping->cmd_handler,
		default_args
	};

	sys_free_mem(args);
	return 0;
}
