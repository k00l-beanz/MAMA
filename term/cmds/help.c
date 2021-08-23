#include <lib/out.h>

/*
 * Procedure: cmd_help()
 * Description: Returns syntax for specified command.
*/
int cmd_help(char *arg_str) {
	(void)arg_str;

	println("You're inside the handler function for the 'help' command!", 58);
	println("Someone should implement me :(", 30);
	return 0;
}
