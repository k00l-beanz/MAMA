#ifndef OUT_H
#define OUT_H

/**
 * Prints help message for command
 * 
 * Prints out a help message and basic syntax for
 * a specific command
 * 
 * @param command Command which the user needs basic information and syntax for
 * 
 * @return 1 upon success, -1 upon error
*/
int cmd_help(char * command);

/**
 * Help page for gettime() method
 * 
 * Prints out the name, usage, return and description for the gettime() method.
*/
void gettimeHelp();

/**
 * Help page for settime() method
 * 
 * Prints out the name, usage, and description for the settime() method.
*/
void settimeHelp();

/**
 * Help page for the getdate() method
 * 
 * Prints out the name, usage, return and description for the getdate() method. 
*/
void getdateHelp();

/**
 * Help page for the setdate() method
 * 
 * Prints out the name, usage, and description for the setdate() method.
*/
void setdateHelp();

/**
 * Help page for the help command
 * 
 * Prints out the name, usage, return and description for the help command. 
*/
void helpHelp();

/**
 * Help page for the shutdown command
 * 
 * Prints out the name, usage, and description for the shutdown system command. 
 *
*/
void shutdownHelp();

/**
 * Displays a list of common system commands
 * 
 * Displays a list of common system commands for the user.
*/
void helpList();

/**
 * Help page for the version command
 * 
 * Displays the current verson of the system. 
 * 
*/
void versionHelp();

/**
 * Help page for createpcb
 * 
 * Displays the createpcb help page
 * 
*/
void createpcbHelp();

/**
 * Help page for deletepcb
 * 
 * Displays the deletepcb help page 
*/
void deletepcbHelp();

/**
 * Help page for showpcb
 * 
 * Displays the showpcb help page
*/
void showpcbHelp();

/**
 * Help page for showallpcb
 * 
 * Displays the showallpcb help page
*/
void showallpcbHelp();

/**
 * Help page for showreadypcb
 * 
 * Displays the showreadypcb help page
*/
void showreadypcbHelp();

/**
 * Help page for showblockedpcb
 * 
 * Displays the showblockedpcb help page
*/
void showblockedpcbHelp();

/**
 * Help page for block
 * 
 * Displays the block help page
*/
void blockHelp();

/**
 * Help page for unblock
 * 
 * Displays te unblock help page
*/
void unblockHelp();

/**
 * Help page for setpriority
 * 
 * Displays the setpriority help page
*/
void setpriorityHelp();

/**
 * Help page for resume
 * 
 * Displays the resume help page
*/
void resumeHelp();

/**
 * Help page for suspend
 * 
 * Displays the suspend help page
*/
void suspendHelp();

/**
 * Help page for loadr3
 * 
 * Displays the loadr3 help page 
*/
void loadr3Help();

/**
 * Help page for setalarm
 * 
 * Displays the setalarm help page
*/
void setalarmHelp();

/**
 * Help page for showalarm
 * 
 * Displays the showalarm help page 
*/
void showalarmsHelp();

/**
 * Help page for freealarm
 * 
 * Displays the freealarm help page 
 * 
*/
void freealarmHelp();

/**
 * Help page for resumeallpcb
 * 
 * Displays the resumeallpcb help page 
*/
void resumeallHelp();

int print(char *, int);
int printc(char);
int println(char *, int);
void printf(char *, ...);
int read(char *, int);

#endif
