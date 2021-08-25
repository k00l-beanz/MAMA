#include <lib/out.h>


int cmd_help(char *command) {

	if (strcmp(command,"gettime") == 0) {
		gettimehelp();
		return 1;
	} 

	println("You're inside the handler function for the 'help' command!", 58);
	println("Someone should implement me :(", 30);
	return 0;
}

void gettimehelp() {
	println("Name: gettime\nSynopsis: gettime\nDescription: Get the current system time", 72);
}