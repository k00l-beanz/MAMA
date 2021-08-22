#include <term/cmds/help.c>
#include <term/cmds/shutdown.c>

#define MAX_CMD_STRING_LEN 100
#define MAX_CMD_NAME_LEN 30

typedef int (*cmd_func_t)();

typedef struct cmd_mapping {
	char *cmd_name;
	cmd_func_t cmd_handler;
} cmd_mapping;

const cmd_mapping cmd_mappings[] = {
	{
		"help",
		&cmd_help
	},
	{
		"shutdown",
		&cmd_shutdown
	},
	{ NULL }
};

int commhand();
