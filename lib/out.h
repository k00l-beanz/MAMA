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
 * Help message for gettime() method
 * 
 * Prints out the name, usage, return and description for the gettime() method.
*/
void gettimeHelp();

/**
 * Help message for settime() method
 * 
 * Prints out the name, usage, and description for the settime() method.
*/
void settimeHelp();

/**
 * Help message for the getdate() method
 * 
 * Prints out the name, usage, return and description for the getdate() method. 
*/
void getdateHelp();

/**
 * Help message for the setdate() method
 * 
 * Prints out the name, usage, and description for the setdate() method.
*/
void setdateHelp();

/**
 * Help message for the help command
 * 
 * Prints out the name, usage, return and description for the help command. 
*/
void helHelp();


int print(char *, int);
int printc(char);
int println(char *, int);
int read(char *, int);

#endif
