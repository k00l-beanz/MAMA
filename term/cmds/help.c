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
	else if (strcmp(command, " setdate") == 0) {
		setdateHelp();
		return 1;
	}
	else if (strcmp(command, " help") == 0) {
		helpHelp();
		return 1;
	}
	else if (strcmp(command, " shutdown") == 0) {
		shutdownHelp();
		return 1;
	}
	else if (strcmp(command, " list") == 0) {
		helpList();
		return 1;
	}
	else if (strcmp(command, " version") == 0) {
		versionHelp();
		return 1;
	}
	else {
		print("Trying to get help with a command?\nType: help [COMMAND]\nTo see a list of common commands, Type: help list\n",106);
	}

	return 1;
}

void versionHelp() {
	print("NAME\n\tversion\n\nUSAGE\n\tversion\n\nDESCRIPTION\n\tDisplay the current version of the system\n\nRETURN\n\tCurrent version of the system\n\n",126);
}

void helpList() {
	print("------------ ------------\n"
		  "| SYSTEM   | | TIME     |\n"
		  "------------ ------------\n"
		  "| help     | | getdate  |\n"
		  "| version  | | setdate  |\n"
		  "| shutdown | | gettime  |\n"
		   "------------ | settime  |\n"
			 		   "\t     ------------\n",191);
}

void shutdownHelp() {
	print("NAME\n\t"
		  "shutdown\n\n"
		  "USAGE\n\t"
		  "shutdown\n\n"
		  "DESCRIPTION\n\t"
		  "Shutdown the system. This will ask for confirmation before shutting down.\n\n",121);
}

void helpHelp() {
	print("NAME\n\t"
		  "help\n\n"
		  "USAGE\n\t"
		  "help [COMMAND]\n\n"
		  "DESCRIPTION\n\t"
		  "See the help pages for the specified command\n\nRETURN\n\t"
		  "The help page for the inputed command\n\n"
		  "EXAMPLE\n\t"
		  "help shutdown\n\n",166);
}


void setdateHelp() {
	print("NAME\n\t"
		  "setdate\n\n"
		  "USAGE\n\t"
		  "setdate [MONTH.DAY.YEAR]\n\n"
		  "DESCRIPTION\n\t"
		  "Set the date of the system. Months and days of the week correspond to decimal values. See Table 1 below for the correct mapping.\n\t"
		  "The maximum value for day and year are 31 and 99 (2099) respectively while the minimum year is 10 (2010).\n\n"
		  "\tTABLE 1: MONTHS\n"
		  "\t--------------------\n"
		  "\t| January\t1  |\n"
		  "\t| February\t2  |\n"
		  "\t| March\t\t3  |\n"
		  "\t| April\t\t4  |\n"
		  "\t| May\t\t5  |\n"
		  "\t| June\t\t6  |\n"
		  "\t| July\t\t7  |\n"
		  "\t| August\t8  |\n"
		  "\t| September\t9  |\n"
		  "\t| October\t10 |\n"
		  "\t| November\t11 |\n"
		  "\t| December\t12 |\n"
		  "\t--------------------\n\n"
		  "EXAMPLE\n\t"
		  "Thursday, August 26, 2021\n\t"
		  "MONTH = 8 DAY = 26 YEAR = 21\n\t"
		  "setdate 8.26.21\n\n",773);
}

void getdateHelp() {
	print("NAME\n\t"
		  "getdate\n\n"
		  "USAGE\n\t"
		  "getdate\n\n"
		  "DESCRIPTION\n\t"
		  "Display the current system date\n\n"
		  "RETURN\n\t"
		  "Returns the current date of the system in DAYOFWEEK, MONTH DAY, YEAR format.\n\n"
		  "EXAMPLE\n\t"
		  "Thursday, August 26, 2021\n\n",199);
}

void gettimeHelp() {
	print("NAME\n\t"
		  "gettime\n\n"
		  "USAGE\n\t"
		  "gettime\n\n"
		  "DESCRIPTION\n\t"
		  "Display the current system time\n\n"
		  "RETURN\n\t"
		  "Returns the current time of the system in HOUR:MINUTE:SECOND format\n\n"
		  "EXAMPLE\n\t"
		  "08:50:23\n\n", 173);
}

void settimeHelp() {
	print("NAME\n\t"
		  "settime\n\n"
		  "USAGE\n\t"
		  "settime [HOUR.MINUTE.SECOND]\n\n"
		  "DESCRIPTION\n\t"
		  "Set the time of the system. Since time is displayed in military time the maximum value for HOUR you can set is 23.\n\t"
		  "Both MINUTE and SECOND have a maximum value of 59.\n\n"
		  "EXAMPLE\n\t"
		  "22:13:34\n\t"
		  "HOUR = 22 MINUTE = 13 SECOND = 34\n\t"
		  "settime 22.13.34\n\n",289);
}