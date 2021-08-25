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
 * Help message for gettime
 * 
 * Prints out the help message for gettime
 * 
*/
void gettimehelp();

int print(char *, int);
int printc(char);
int println(char *, int);
int read(char *, int);

#endif
