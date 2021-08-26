#include <lib/out.h>


int cmd_help(char *command) {

	if (strcmp(command," gettime") == 0) {
		gettimeHelp();
		return 1;
	} 
	else if (strcmp(command," settime") == 0) {
		settimeHelp();
		return 1;
	} 
	else if (strcmp(command," getdate") == 0){
		getdateHelp();
		return 1;
	}
	else {
		print("Trying to get help with a command?\nType: help [COMMAND]\n",56);
	}

	return 1;
}

void getdateHelp() {

}

void gettimeHelp() {
	print("NAME\n\t"
		  "gettime\n\n"
		  "USAGE\n\t"
		  "gettime\n\n"
		  "DESCRIPTION\n\t"
		  "Display the current system time\n\n"
		  "RETURN\n\t"
		  "Returns the current time of the system in HOUR:MINUTE:SECOND format\n", 153);
}

void settimeHelp() {
	print("NAME\n\t"
		  "settime\n\n"
		  "USAGE\n\t"
		  "settime [HOUR.MINUTE.SECOND]\n\n"
		  "DESCRIPTION\n\t"
		  "Set the time of the system. Since time is displayed in military time the maximum value for HOUR you can set is 23.\n\t"
		  "Both MINUTE and SECOND have a maximum value of 59.\n\n\t"
		  "Example:\n\t\t"
		  "HOUR = 22 MINUTE = 13 SECOND = 34\n\n\t\t\t"
		  "22.13.34\n\n",293);
}