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
		//helpHelp();
		return 1;
	}
	else {
		print("Trying to get help with a command?\nType: help [COMMAND]\n",56);
	}

	return 1;
}

/*
void helpHelp() {
	print()
}
*/

void setdateHelp() {
	print("NAME\n\t"
		  "setdate\n\n"
		  "USAGE\n\t"
		  "setdate [DAYOFWEEK.MONTH.DAY.YEAR]\n\n"
		  "DESCRIPTION\n\t"
		  "Set the date of the system. Months and days of the week correspond to decimal values. See Table 1 and Table 2 below for the correct mapping.\n\n"
		  "TABLE 1: MONTHS\t\tTABLE 2: DAYS OF WEEK\n"
		  "--------------------\t-------------------\n"
		  "| January\t1  |\t| Sunday\t1 |\n"
		  "| February\t2  |\t| Monday\t2 |\n"
		  "| March\t\t3  |\t| Tuesday\t3 |\n"
		  "| April\t\t4  |\t| Wednesday\t4 |\n"
		  "| May\t\t5  |\t| Thursday\t5 |\n"
		  "| June\t\t6  |\t| Friday\t6 |\n"
		  "| July\t\t7  |\t| Saturday\t7 |\n"
		  "| August\t8  |\t-------------------\n"
		  "| September\t9  |\n"
		  "| October\t10 |\n"
		  "| November\t11 |\n"
		  "| December\t12 |\n"
		  "--------------------\n\n"
		  "EXAMPLE\n\t"
		  "Thursday, August 26, 2021\n\t"
		  "DAY OF WEEK = 5 MONTH = 8 DAY = 26 YEAR = 21\n\t"
		  "setdate 5.8.26.21\n\n",710);
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
		  "HOUR = 22 MINUTE = 13 SECOND = 34\n\t"
		  "settime 22.13.34\n\n",289);
}