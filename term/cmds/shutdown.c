#include <lib/out.h>

/*
 * Procedure: cmd_shutdown()
 * Description: Shutdown application
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
