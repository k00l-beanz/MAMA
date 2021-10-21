#include <lib/out.h>

/**
 * Handler for calls to the shutdown command. Prompts for user confirmation before shutting the system down.
 *
 * @param arg_str The arguments passed to the shutdown command. Unused by the handler.
 *
 * @return The exit code of the command, indicating whether or not the user confirmed the request to shutdown the system. Returns 0 if the user confirmed the request, 1 otherwise.
 */
int cmd_shutdown(char *arg_str) {
	(void)arg_str;

	/* Confirmation to shutdown the system */
	println("Are you sure you'd like to shutdown the system? [y/n]",53);

	char *input = "\0\0\0";
	read(input, 2);
	println("", 0);

	if (strcmp(input, "y") == 0) {
		return 0;
	}
	return 1;
}
