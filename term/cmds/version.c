#include <lib/out.h>

/**
 * Handler for the version command. Prints the current version of the operating system.
 *
 * @param arg_str The arguments passed to the version command. Unused by the handler.
 *
 * @return The exit code of the command, always 0.
 */
int cmd_version(char *arg_str) {
	(void)arg_str;
	
	// printing the OS version number
	char *versionOs= "\nCurrent Version: R5\n";
	println(versionOs,20);
	
	return 0;
}
