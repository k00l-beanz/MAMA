#include "syntax.h"
#include "utils.h"

int changes_state(char, enum SyntaxState, enum SyntaxState *);

enum SyntaxState get_state(char c, enum SyntaxState cur_state) {
	changes_state(c, cur_state, &cur_state);
	return cur_state;
}

int changes_state(char c, enum SyntaxState cur_state, enum SyntaxState *next_state) {
	if(c == '"') {
		switch(cur_state) {
			case DOUBLE_QUOTE_STRING:
				*next_state = DOUBLE_QUOTE_STRING_END_QUOTE;
				return 1;
			case SINGLE_QUOTE_STRING:
				return 0;
			default:
				*next_state = DOUBLE_QUOTE_STRING;
				return 1;
		}
	} else if(c == '\'') {
		switch(cur_state) {
			case SINGLE_QUOTE_STRING:
				*next_state = SINGLE_QUOTE_STRING_END_QUOTE;
				return 1;
			case DOUBLE_QUOTE_STRING:
				return 0;
			default:
				*next_state = SINGLE_QUOTE_STRING;
				return 1;
		}
	} else if(c == '-') {
		switch(cur_state) {
			case DEFAULT:
			case DOUBLE_QUOTE_STRING_END_QUOTE:
			case SINGLE_QUOTE_STRING_END_QUOTE:
				*next_state = PARAM_NAME;
				return 1;
			default:
				return 0;
		}
	} else if(isspace(&c)) {
		switch(cur_state) {
			case PARAM_NAME:
			case CMD_NAME:
			case PARAM_VALUE:
			case DOUBLE_QUOTE_STRING_END_QUOTE:
			case SINGLE_QUOTE_STRING_END_QUOTE:
				*next_state = DEFAULT;
				return 1;
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
			case CMD_NAME_OR_LEADING_WHITESPACE:
			case DEFAULT:
			default:
				return 0;
		}
	} else if(is_name_char(c)) {
		switch(cur_state) {
			case CMD_NAME_OR_LEADING_WHITESPACE:
				*next_state = CMD_NAME;
				return 1;
			case DOUBLE_QUOTE_STRING_END_QUOTE:
			case SINGLE_QUOTE_STRING_END_QUOTE:
			case DEFAULT:
				*next_state = PARAM_VALUE;
				return 1;
			default:
				return 0;
		}
	} else if(c == '\0') {
		*next_state = END_OF_INPUT;
		return 1;
	} else {
		switch(cur_state) {
			case DOUBLE_QUOTE_STRING_END_QUOTE:
			case SINGLE_QUOTE_STRING_END_QUOTE:
			case DEFAULT:
				*next_state = PARAM_VALUE;
				return 1;
			default:
				return 0;
		}
	}
}
