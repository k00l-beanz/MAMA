/* the logic for each command the user has to run is contained in a separate file in term/cmds
 * each file should contain a function to run this command and possibly any helper functions the command needs to run
 * include each of these files below - make sure to add an #include directive if you write a new command
 */
#ifndef COMMHAND_H
#define COMMHAND_H

#define MAX_CMD_STRING_LEN 100   /// The maximum length of the entire command string entered by the user, including command name and all args
#define MAX_CMD_NAME_LEN 30      /// The maximum length of the name of a command
#define MAX_CMD_HIST_LEN 20      /// The maximum number of entries kept in the user's command history
#define MAX_CMD_ARG_NAME_LEN 30  /// The maximum length of the name of a named argument to a command
#define MAX_CMD_ARG_VALUE_LEN 40 /// The maximum length of the value of a single argument to a command, named or unnamed
#define MAX_CMD_FLAG_COUNT 10
#define MAX_CMD_NAMED_ARG_COUNT 10
#define MAX_CMD_UNNAMED_ARG_COUNT 10

#define MAX_CMD_COUNT 200  /// The maximum number of commands that can exist in the system, including both built-in commands and user-defined aliases

void commhand();
#endif
