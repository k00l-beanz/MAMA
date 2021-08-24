#include <include/string.h>
#include <modules/mpx_supt.h>
#include "visuals/colorize.c"
#include "history.c"
#include "commhand.h"
#include "commands.h"
#include "visuals/syntax.h"
#include "visuals/hints.h"
#include <lib/out.c>
#include "dnt/dnt.c"


typedef int (*cmd_func_t)(char *);

typedef struct cmd_mapping {
        char *cmd_name;
        cmd_func_t cmd_handler;
} cmd_mapping;

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
const cmd_mapping cmd_mappings[] = {
        {
                "help",
                &cmd_help
        },
        {
                "shutdown",
                &cmd_shutdown
        },
        {
        	"version",
        	&cmd_version
        },
        {
        	"gettime",
        	&gettime
        },
        {
        	"settime",
        	&settime
        },
        {
        	"getdate",
        	&getdate
        },
        {
        	"setdate",
        	&setdate
        },
	{
		"echo",
		&cmd_echo
	},
        { NULL } // sentinel for end-of-array
};

int is_name_char(char);
void extract_cmd_name(char *, char *, int *);
cmd_func_t fetch_cmd_handler(char *);

/*
 * Procedure: commhand()
 * Description: Takes user input and interprets the command
*/
int commhand() {
	syntax_init();

	char cmd_name[MAX_CMD_NAME_LEN + 1];
	int cmd_name_len = 0;

	int running = 1;
	
	/* Driver loop */
	while(running) {
		/* Prints prompt */
		display_fg_color(GREEN);
		print("~--> ", 5);
		display_reset();

		/* Reads in user input */
		char *cmd_str = hist_next_frame();
		read(cmd_str, MAX_CMD_STRING_LEN);
		println("", 0);
		display_reset();

		/* Extract command name (typically first word) from command string */
		extract_cmd_name(cmd_str, cmd_name, &cmd_name_len);


		cmd_func_t handler = fetch_cmd_handler(cmd_name);


		if(handler != NULL) {
			(*handler)(cmd_str + cmd_name_len);
		} else {
			print("Unrecognized command: ", 22);
			println(cmd_name, cmd_name_len);
		}

		/* Command shutdown kills driver loop */
		if(strcmp(cmd_name, "shutdown") == 0)
			running = 0;
	}

	return 0;
}



cmd_func_t fetch_cmd_handler(char *cmd_name) {	
	int i = 0;
	while(cmd_mappings[i].cmd_name != NULL) {
		if(strcmp(cmd_name, cmd_mappings[i].cmd_name) == 0)
			return (cmd_func_t)cmd_mappings[i].cmd_handler;
		i++;
	}
	return NULL;
}

/*
 * Procedure: extract_cmd_name
 * Description: Extracts the command (typically the first word) from the command
 *  string.
*/
void extract_cmd_name(char *cmd_str, char *cmd_name, int *cmd_name_len) {
	*cmd_name_len = 0;
	int i;
        for(i = 0; i < MAX_CMD_NAME_LEN && is_name_char(cmd_str[i]); i++) {
                *cmd_name_len += 1;
		cmd_name[i] = cmd_str[i];
	}

	cmd_name[*cmd_name_len] = '\0';
}

/*
 * Procedure: is_name_char
 * Description: Checks for invalid characters
*/
int is_name_char(char c) {
	return  (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c >= '0' && c <= '9') ||
		(c == '_');
}
