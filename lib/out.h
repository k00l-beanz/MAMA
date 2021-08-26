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


int print(char *, int);
int printc(char);
int println(char *, int);
int read(char *, int);

#endif
