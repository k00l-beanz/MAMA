#include "../syntax.h"
#include "../syntax.c"
#include "syntax_highlight.h"
#include "../commhand.h"
#include "colorize.h"
#include "hints.c"
#include "../utils.c"
#include <include/string.h>

enum SyntaxState states[MAX_SYNTAX_SWITCHES]; /// Array of all the states the cursor has been in as the user has typed. Entries correspond to entries in switch_indexes.
int switch_indexes[MAX_SYNTAX_SWITCHES]; /// Array of indexes the cursor was at when the corresponding syntax state in states was switched to.
int newest_switch; /// The largest and most recent valid index in states and switch_indexes.

int enabled = 0; /// Whether or not syntax highlighting is enabled as the user types.

void switch_to(enum SyntaxState, int, int);
void color_for(enum SyntaxState);
void get_state_at(int, int *);

/**
 * Initializes internal data structures needed for syntax highlighting.
 */
void syntax_init() {
	states[0] = CMD_NAME_OR_LEADING_WHITESPACE;
	switch_indexes[0] = 0;
	newest_switch = 0;
	int i; // >:(
	for(i = 1; i < MAX_SYNTAX_SWITCHES; i++) {
		states[i] = DEFAULT;
		switch_indexes[i] = MAX_CMD_STRING_LEN + 1;
	}
}

/**
 * Enables syntax highlighting as the user types.
 */
void syntax_enable_highlighting() {
	enabled = 1;
}

/**
 * Disables syntax highlighting as the user types.
 */
void syntax_disable_highlighting() {
	enabled = 0;
	color_for(DEFAULT);
}

/**
 * Adjusts the terminal color assuming the specified character will immediately be written to the screen at the specified index.
 *
 * @param c The next character that will be output to the screen.
 * @param index The index of the cursor.
 */
void syntax_handle_char(char c, int index) {
	if(!enabled)
		return;

	int record_index;
	get_state_at(index, &record_index);
	if(switch_indexes[record_index] == index && record_index != 0) {
		switch_indexes[record_index] = MAX_CMD_STRING_LEN + 1;
		record_index--;
	}
	enum SyntaxState cur_state = states[record_index];

	enum SyntaxState next_state;
	if(changes_state(c, cur_state, &next_state)) {
		switch_to(next_state, index, record_index);
	} else {
		color_for(cur_state);
	}
}

/**
 * Switches to the specified syntax state. Used internally by syntax_handle_char.
 *
 * @param state The syntax state being switched to.
 * @param index The index in the user's input at which this switch occurs.
 * @param record_index The index in the internal data structures states and switch_indexes at which to write this switch to.
 */
void switch_to(enum SyntaxState state, int index, int record_index) {
	if(record_index + 1 < MAX_SYNTAX_SWITCHES) {
		record_index++;
		states[record_index] = state;
		color_for(state);
		switch_indexes[record_index] = index;
		newest_switch = record_index;
	}
}

/**
 * Prints the ANSI color code for the specified syntax state. Used internally by syntax_handle_char.
 *
 * @param state The syntax state for which to print the correct color code to the terminal for.
 */
void color_for(enum SyntaxState state) {
	switch(state) {
		case DOUBLE_QUOTE_STRING:
		case DOUBLE_QUOTE_STRING_END_QUOTE:
			display_fg_color(SYNTAX_COLOR_DOUBLE_QUOTE_STRING);
			break;
		case SINGLE_QUOTE_STRING:
		case SINGLE_QUOTE_STRING_END_QUOTE:
			display_fg_color(SYNTAX_COLOR_SINGLE_QUOTE_STRING);
			break;
		case PARAM_NAME:
			display_fg_color(SYNTAX_COLOR_PARAM_NAME);
			//display_italicize();
			break;
		case PARAM_VALUE:
			display_fg_color(SYNTAX_COLOR_PARAM_VALUE);
			break;
		case CMD_NAME:
		case CMD_NAME_OR_LEADING_WHITESPACE:
			display_fg_color(SYNTAX_COLOR_CMD_NAME);
			break;
		case DEFAULT:
		default:
			display_fg_color(SYNTAX_COLOR_DEFAULT);
			break;
	}
}

/**
 * Retrieves the index in the states and switch_indexes data structures corresponding to the specified cursor index. Used internally by syntax_handle_char.
 *
 * @param index The index of the cursor.
 * @param index_of_state_in_record A pointer to the index in the states and switch_indexes data structures corresponding to the specified cursor index. Will be updated to point to the correct index in the data structures.
 */
void get_state_at(int index, int *index_of_state_in_record) {
	int i;
	*index_of_state_in_record = 0;
	for(i = 0; i <= newest_switch; i++) {
		if(switch_indexes[i] > index)
			break;
		*index_of_state_in_record = i;
	}
}


