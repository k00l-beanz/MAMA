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
	else if (strcmp(command, " createpcb") == 0) {
		createpcbHelp();
		return 1;
	}
	else if (strcmp(command, " deletepcb") == 0) {
		deletepcbHelp();
		return 1;
	}
	else if (strcmp(command, " showpcb") == 0) {
		showpcbHelp();
		return 1;
	}
	else if (strcmp(command, " showallpcb") == 0) {
		showallpcbHelp();
		return 1;
	}
	else if (strcmp(command, " showreadypcb") == 0) {
		showreadypcbHelp();
		return 1;
	}
	else if (strcmp(command, " showblockedpcb") == 0) {
		showblockedpcbHelp();
		return 1;
	}
	else if (strcmp(command, " blockpcb") == 0) {
		blockHelp();
		return 1;
	}
	else if (strcmp(command, " unblockpcb") == 0) {
		unblockHelp();
		return 1;
	}
	else if (strcmp(command, " setprioritypcb") == 0) {
		setpriorityHelp();
		return 1;
	}
	else if (strcmp(command, " resumepcb") == 0) {
		resumeHelp();
		return 1;
	}
	else if (strcmp(command, " suspendpcb") == 0) {
		suspendHelp();
		return 1;
	}
	else if (strcmp(command, " loadr3") == 0) {
		loadr3Help();
		return 1;
	}
	else if (strcmp(command, " setalarm") == 0) {
		setalarmHelp();
		return 1;
	}
	else if (strcmp(command, " showalarms") == 0) {
		showalarmsHelp();
		return 1;
	}
	else if (strcmp(command, " freealarm") == 0) {
		freealarmHelp();
		return 1;
	}
	else if (strcmp(command, " resumeallpcb") == 0) {
		resumeallHelp();
		return 1;
	}
	else if (strcmp(command, " showalloc") == 0) {
		showallocHelp();
		return 1;
	}
	else if (strcmp(command, " showfree") == 0) {
		showfreeHelp();
		return 1;
	}
	else if (strcmp(command, " isempty") == 0) {
		isemptyHelp();
		return 1;
	}
	else if (strcmp(command, " clear") == 0) {
		clearHelp();
		return 1;
	}
	else if (strcmp(command, " alias") == 0) {
		aliasHelp();
		return 1;
	}
	else {
		print("Trying to get help with a command?\nType: help [COMMAND]\nTo see a list of common commands, Type: help list\n",106);
	}

	return 1;
}


/******************************************************/
/********************** SYSTEM ************************/
/******************************************************/

void versionHelp() {
	print("NAME\n\tversion\n\nUSAGE\n\tversion\n\nDESCRIPTION\n\tDisplay the current version of the system\n\nRETURN\n\tCurrent version of the system\n\n",126);
}

void helpList() {
	print(" ------------ ------------ ------------------ -------------- --------------\n"
		  " | SYSTEM   | | TIME     | | PCB            | | ALARM      | | HEAP       |\n"
		  " ------------ ------------ ------------------ -------------- --------------\n"
		  " | help     | | getdate  | | createpcb      | | setalarm   | | showalloc  |\n"
		  " | version  | | setdate  | | deletepcb      | | showalarms | | showfree   |\n"
		  " | shutdown | | gettime  | | showpcb        | | freealarm  | | isempty    |\n"
		  " | clear    | | settime  | | showallpcb     | -------------- --------------\n"
		  " | alias    | ------------ | showreadypcb   |\n"
		  " ------------              | showblockedpcb |\n"
		  "                           | blockpcb       |\n"
		  "                           | unblockpcb     |\n"
		  "                           | setprioritypcb |\n"
		  "                           | resumepcb      |\n"
		  "                           | suspendpcb     |\n"
		  "                           | resumeallpcb   |\n"
		  "                           | loadr3         |\n"
		  "                           ------------------\n",191);
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

void clearHelp() {
	printf("NAME\n\t"
		   "clear\n\n"
		   "USAGE\n\t"
		   "clear\n\n"
		   "DESCRIPTION\n\t"
		   "Clears the terminal screen.\n\n");
}

void aliasHelp() {
	printf("NAME\n\t"
		   "alias\n\n"
		   "USAGE\n\t"
		   "alias [ALIAS] [COMMAND]\n\n"
		   "DESCRIPTION\n\t"
		   "Allows user to rename currently existing commands\n\n"
		   "EXAMPLE\n\t"
		   "alias mama shutdown\n\n");
}

/******************************************************/
/********************** TIME **************************/
/******************************************************/
void setdateHelp() {
	print("NAME\n\t"
		  "setdate\n\n"
		  "USAGE\n\t"
		  "setdate [MONTH.DAY.YEAR]\n\n"
		  "DESCRIPTION\n\t"
		  "Set the date of the system. Months and days of the week correspond to decimal values. See Table 1 below for the correct mapping.\n\t"
		  "The maximum value for day and year are 31 and 99 (2099) respectively while the minimum year is 10 (2010).\n\n"
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

/******************************************************/
/********************** PCB ***************************/
/******************************************************/

void createpcbHelp() {
	print("NAME\n\t"
		  "createpcb\n\n"
		  "USAGE\n\t"
		  "createpcb [PCB_NAME.PROCESS_CLASS.PRIORITY]\n\n"
		  "DESCRIPTION\n\t"
		  "Creates a new, unique Process Control Block (PCB) in memory.\n\t"
		  "PCB's can have a name up to 32 characters long.\n\t"
		  "Processes are either:\n\t\t"
		  "SYSTEM_PROCESS = 0\n\t\t"
		  "APPLICATION = 1\n\t"
		  "Processes are assigned a priority from 0 (low priority) up to and including 9 (high priority)\n\n"
		  "EXAMPLE\n\t"
		  "PCB_NAME = mama\n\t"
		  "PROCESS CLASS = 0 (System Process)\n\t"
		  "PRIORITY = 9 (High)\n\t"
		  "createpcb mama.0.9\n\n",1);
}

void deletepcbHelp() {
	print("NAME\n\t"
		  "deletepcb\n\n"
		  "USAGE\n\t"
		  "deletepcb [PCB_NAME]\n\n"
		  "DESCRIPTION\n\t"
		  "Will delete the given PCB from its queue and free associated memory.\n\n"
		  "EXAMPLE\n\t"
		  "deletepcb mama\n\n",1);

}

void showpcbHelp() {
	print("NAME\n\t"
		  "showpcb\n\n"
		  "USAGE\n\t"
		  "showpcb [PCB_NAME]\n\n"
		  "DESCRIPTION\n\t"
		  "Display information of the PCB. The following information will be displayed:\n\t\t"
		  "Name\n\t\t"
		  "Class\n\t\t"
		  "State\n\t\t"
		  "Suspended Status\n\t\t"
		  "Priority\n\n"
		  "EXAMPLE\n\t"
		  "showpcb mama\n\n",1);
}

void showallpcbHelp() {
	print("NAME\n\t"
		  "showallpcb\n\n"
		  "USAGE\n\t"
		  "showallpcb\n\n"
		  "DESCRIPTION\n\t"
		  "Display all information for every PCB in the Ready and Blocked queues\n\n"
		  "EXAMPLE\n\t"
		  "showallpcb\n\n",1);
}

void showreadypcbHelp() {
	print("NAME\n\t"
		  "showreadypcb\n\n"
		  "USAGE\n\t"
		  "showreadypcb\n\n"
		  "DESCRIPTION\n\t"
		  "Display all information for every PCB in the Ready queue\n\n"
		  "EXAMPLE\n\t"
		  "showreadypcb\n\n",1);
}

void showblockedpcbHelp() {
	print("NAME\n\t"
		  "showblockedpcb\n\n"
		  "USAGE\n\t"
		  "showblockedpcb\n\n"
		  "DESCRIPTION\n\t"
		  "Display all information for every PCB in the Blocked queue\n\n"
		  "EXAMPLE\n\t"
		  "showblockedpcb\n\n",1);
}

void blockHelp() {
	print("NAME\n\t"
		  "blockpcb\n\n"
		  "USAGE\n\t"
		  "blockpcb [PCB_NAME]\n\n"
		  "DESCRIPTION\n\t"
		  "Sets the specified PCB into a blocked state and inserts it into the Blocked queue\n\n"
		  "EXAMPLE\n\t"
		  "blockpcb mama\n\n",1);
}

void unblockHelp() {
	print("NAME\n\t"
		  "unblockpcb\n\n"
		  "USAGE\n\t"
		  "unblockpcb [PCB_NAME]\n\n"
		  "DESCRIPTION\n\t"
		  "Sets the specified PCB into an unblocked state and inserts it into the Ready queue\n\n"
		  "EXAMPLE\n\t"
		  "unblockpcb mama\n\n",1);
}

void setpriorityHelp() {
	print("NAME\n\t"
		  "setprioritypcb\n\n"
		  "USAGE\n\t"
		  "setprioritypcb [PCB_NAME.NEW_PRIORITY]\n\n"
		  "DESCRIPTION\n\t"
		  "Sets a new priority to the specified PCB\n\n"
		  "EXAMPLE\n\t"
		  "setprioritypcb mama.2\n\n",1);
}

void resumeHelp() {
	print("NAME\n\t"
		  "resumepcb\n\n"
		  "USAGE\n\t"
		  "resumepcb [PCB_NAME]\n\n"
		  "DESCRIPTION\n\t"
		  "Resumes the suspended PCB\n\n"
		  "EXAMPLE\n\t"
		  "resumepcb mama\n\n",1);
}

void suspendHelp() {
	print("NAME\n\t"
		  "suspendpcb\n\n"
		  "USAGE\n\t"
		  "suspendpcb [PCB_NAME]\n\n"
		  "DESCRIPTION\n\t"
		  "Suspends the running PCB\n\n"
		  "EXAMPLE\n\t"
		  "suspendpcb mama\n\n",1);
}

/******************************************************/
/********************** R4 ****************************/
/******************************************************/

void loadr3Help() {
	print("NAME\n\t"
		  "loadr3\n\n"
		  "USAGE\n\t"
		  "loadr3\n\n"
		  "DESCRIPTION\n\t"
		  "Loads all test processes into the READY queue in a SUSPENDED_READY state. Each process will be\n\t"
		  "treated as an APPLICATION with a priority of 4\n\n",1);
}

void setalarmHelp() {
	printf("NAME\n\t"
		   "setalarm\n\n"
		   "USAGE\n\t"
		   "setalarm hour:minute,message\n\n"
		   "DESCRIPTION\n\t"
		   "Sets an alarm for the user with a custom message.\n\t"
		   "NOTE: The message must be one word\n\n"
		   "EXAMPLE\n\t"
		   "hour = 5\n\t"
		   "minute = 30\n\t"
		   "message = cs-450_class!\n\n\t"
		   "setalarm 5:30,cs-450_class!\n\n");
}

void showalarmsHelp() {
	printf("NAME\n\t"
		   "showalarms\n\n"
		   "USAGE\n\t"
		   "showalarms\n\n"
		   "DESCRIPTION\n\t"
		   "Displays all alarms and their messages\n\n");
}

void freealarmHelp() {
	printf("NAME\n\t"
		   "freealarm\n\n"
		   "USAGE\n\t"
		   "freealarm hour:minute\n\n"
		   "DESCRIPTION\n\t"
		   "Removes specified alarm from alarm list\n\n");
}

void resumeallHelp() {
	printf("NAME\n\t"
		   "resumeallpcb\n\n"
		   "USAGE\n\t"
		   "resumeallpcb\n\n"
		   "DESCRIPTION\n\t"
		   "Sets the state of all PCB's in the READY queue to READY\n\n");
}

/******************************************************/
/********************** HEAP **************************/
/******************************************************/

void showallocHelp() {
	printf("NAME\n\t"
		   "showalloc\n\n"
		   "USAGE\n\t"
		   "showalloc\n\n"
		   "DESCRIPTION\n\t"
		   "Shows the memory location and size of all allocated memory control blocks in the heap. The allocated memory\n\t"
		   "control blocks are organized by memory address in ascending order.\n\n");
}

void showfreeHelp() {
	printf("NAME\n\t"
		   "showfree\n\n"
		   "USAGE\n\t"
		   "showfree\n\n"
		   "DESCRIPTION\n\t"
		   "Shows the memory location and size of all free memory control blocks in the heap. The free memory\n\t"
		   "control blocks are organized by memory address in ascending order.\n\n");
}

void isemptyHelp() {
	printf("NAME\n\t"
		   "isempty\n\n"
		   "USAGE\n\t"
		   "isempty\n\n"
		   "DESCRIPTION\n\t"
		   "Shows whether the heap is entirely free memory.\n\n");
}