/* the logic for each command the user has to run is contained in a separate file in term/cmds
 * each file should contain a function to run this command and possibly any helper functions the command needs to run
 * include each of these files below - make sure to add an #include directive if you write a new command
 */
#ifndef COMMHAND_H
#define COMMHAND_H

#define MAX_CMD_STRING_LEN 100
#define MAX_CMD_NAME_LEN 30
#define MAX_CMD_HIST_LEN 20

int commhand();

#endif
