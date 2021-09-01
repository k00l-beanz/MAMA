#ifndef SYNTAX_HIGHLIGHT_H
#define SYNTAX_HIGHLIGHT_H

#define MAX_SYNTAX_SWITCHES 40

#define SYNTAX_COLOR_CMD_NAME CYAN
#define SYNTAX_COLOR_PARAM_NAME MAGENTA
#define SYNTAX_COLOR_PARAM_VALUE WHITE
#define SYNTAX_COLOR_DOUBLE_QUOTE_STRING YELLOW
#define SYNTAX_COLOR_SINGLE_QUOTE_STRING YELLOW
#define SYNTAX_COLOR_DEFAULT WHITE

void syntax_init();
void syntax_enable_highlighting();
void syntax_disable_highlighting();
void syntax_handle_char(char, int);

#endif
