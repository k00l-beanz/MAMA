#include <lib/out.h>

/*
 * Procedure: cmd_version()
 * Description: Returns syntax for specified command.
*/
int cmd_version(char *arg_str) {
	(void)arg_str;
	
	// printing the OS version number
	char *versionOs= "\nCurrent Version: R1\n";
	println(versionOs,20);
	
	return 0;
}
