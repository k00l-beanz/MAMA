#ifndef SYNTAX_H
#define SYNTAX_H

enum SyntaxState {
	CMD_NAME_OR_LEADING_WHITESPACE,
	CMD_NAME,
	PARAM_NAME,
	PARAM_VALUE,
	DOUBLE_QUOTE_STRING,
	DOUBLE_QUOTE_STRING_END_QUOTE,
	SINGLE_QUOTE_STRING,
	SINGLE_QUOTE_STRING_END_QUOTE,
	END_OF_INPUT,
	DEFAULT
};

enum SyntaxState get_state(char, enum SyntaxState);
int changes_state(char, enum SyntaxState, enum SyntaxState *);

#endif
