#include "syntax.h"
#include "../commhand.h"
#include "colorize.h"
#include "hints.c"
#include <include/string.h>

enum SyntaxState states[MAX_SYNTAX_SWITCHES];
int switch_indexes[MAX_SYNTAX_SWITCHES];
int newest_switch;

void switch_to(enum SyntaxState, int, int);
void color_for(enum SyntaxState);
void get_state_at(int, int *);

void syntax_init() {
	states[0] = CMD_NAME;
	switch_indexes[0] = 0;
	newest_switch = 0;
	int i; // >:(
	for(i = 1; i < MAX_SYNTAX_SWITCHES; i++) {
		states[i] = DEFAULT;
		switch_indexes[i] = MAX_CMD_STRING_LEN + 1;
	}
}

void syntax_handle_char(char c, int index) {
	// return; /* TODO: finish syntax highlighting */

	int record_index;
	get_state_at(index, &record_index);
	if(switch_indexes[record_index] == index && record_index != 0) {
		switch_indexes[record_index] = MAX_CMD_STRING_LEN + 1;
		record_index--;
	}
	enum SyntaxState cur_state = states[record_index];

	if(c == '"') {
		switch(cur_state) {
			case DOUBLE_QUOTE_STRING:
				switch_to(DOUBLE_QUOTE_STRING_END_QUOTE, index, record_index);
				break;
			case SINGLE_QUOTE_STRING:
				color_for(cur_state);
				break;
			default:
				switch_to(DOUBLE_QUOTE_STRING, index, record_index);
				break;
		}
	} else if(c == '\'') {
		switch(cur_state) {
			case SINGLE_QUOTE_STRING:
				switch_to(SINGLE_QUOTE_STRING_END_QUOTE, index, record_index);
				break;
			case DOUBLE_QUOTE_STRING:
				color_for(cur_state);
				break;
			default:
				switch_to(SINGLE_QUOTE_STRING, index, record_index);
				break;
		}
	} else if(c == '-') {
		switch(cur_state) {
			case DEFAULT:
			case DOUBLE_QUOTE_STRING_END_QUOTE:
			case SINGLE_QUOTE_STRING_END_QUOTE:
			case PARAM_VALUE:
				switch_to(PARAM_NAME, index, record_index);
				break;
			default:
				color_for(cur_state);
				break;
		}
	} else if(isspace(&c)) {
		switch(cur_state) {
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
				color_for(cur_state);
				break;
			case PARAM_NAME:
				switch_to(PARAM_VALUE, index, record_index);
				break;
			default:
				switch_to(DEFAULT, index, record_index);
				break;
		}
	} else {
		switch(cur_state) {
			case DOUBLE_QUOTE_STRING_END_QUOTE:
			case SINGLE_QUOTE_STRING_END_QUOTE:
				switch_to(DEFAULT, index, record_index);
				break;
			default:
				color_for(cur_state);
				break;
		}
	}
}

void switch_to(enum SyntaxState state, int index, int record_index) {
	if(record_index + 1 < MAX_SYNTAX_SWITCHES) {
		record_index++;
		states[record_index] = state;
		color_for(state);
		switch_indexes[record_index] = index;
		newest_switch = record_index;
	}
}

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
			display_fg_color(SYNTAX_COLOR_CMD_NAME);
			break;
		case DEFAULT:
		default:
			display_fg_color(SYNTAX_COLOR_DEFAULT);
			break;
	}
}

void get_state_at(int index, int *index_of_state_in_record) {
	int i;
	*index_of_state_in_record = 0;
	for(i = 0; i <= newest_switch; i++) {
		if(switch_indexes[i] > index)
			break;
		*index_of_state_in_record = i;
	}
}


