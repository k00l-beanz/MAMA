/* the logic for each command the user has to run is contained in a separate file in term/cmds
 * each file should contain a function to run this command and possibly any helper functions the command needs to run
 * include each of these files below - make sure to add an #include directive if you write a new command
 */

#include <term/cmds/help.c>
#include <term/cmds/shutdown.c>

#define MAX_CMD_STRING_LEN 100
#define MAX_CMD_NAME_LEN 30

typedef int (*cmd_func_t)(char *);

typedef struct cmd_mapping {
	char *cmd_name;
	cmd_func_t cmd_handler;
} cmd_mapping;

/* mapping of command names to their associated functions
 * term/commhand.c uses this to find what function should be ran for a given command
 * eventually, this could also provide other information, such as a function to run to print help text for each command
 * I though this would be cleaner than a bunch of hardcoded if(cmd == "help") ... else if(cmd == "shutdown") ... statements
 * or a massive switch statement with all the command names and functions hardcoded in
 *
 * For new commands, add another struct with the command name as a string and a pointer to its handler function, like below
 * The function to handle the command has to follow the signature for cmd_func_t, which is basically the following
 * 	int my_handler_func(char *arg_str) { ... }
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
	{ NULL } // sentinel for end-of-array
};

int commhand();
