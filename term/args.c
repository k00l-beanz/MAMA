#include "commhand.h"
#include "utils.h"
#include <lib/out.h>
#include <include/string.h>

typedef struct parsed_args {
	int flag_count;
	int named_arg_count;
	int unnamed_arg_count;

	char flags[MAX_CMD_FLAG_COUNT][MAX_CMD_ARG_NAME_LEN + 1];
	char named_arg_names[MAX_CMD_NAMED_ARG_COUNT][MAX_CMD_ARG_NAME_LEN + 1];
	char named_arg_values[MAX_CMD_NAMED_ARG_COUNT][MAX_CMD_ARG_VALUE_LEN + 1];
	char unnamed_args[MAX_CMD_UNNAMED_ARG_COUNT][MAX_CMD_ARG_VALUE_LEN + 1];
} parsed_args;

enum SyntaxState last_state;

int parse_args(char *arg_str, parsed_args *args) {
	skip_ws(&arg_str);
	
	// The only lexeme we have to store and use a lookahead to figure out what to do with is PARAM_NAME,
	// which could either be a flag or the name of a named argument depending on what comes next.
	// Therefore, allocating a buffer with enough room to store argument names is sufficient here, because
	// no other type of lexeme will be written to this buffer.
	char last_token[MAX_CMD_ARG_NAME_LEN + 1];
	int last_token_len = 0;

	last_state = DEFAULT;
	while() {
		enum SyntaxState cur_state;
		while(!changes_state(x, cur_state, &cur_state)) {
			switch(cur_state) {
				case PARAM_NAME:
					skip_ws(&arg_str);

					if(last_state == PARAM_NAME) {
						if(args->flag_count >= MAX_CMD_FLAG_COUNT) {
							print("Bad syntax: commands containing more than ");
							print(itoa(MAX_CMD_FLAG_COUNT));
							print(" flags are not supported.\n");
							return 0;
						}
						
						strcpy(args->flags[args->flag_count++], last_token);
					}

					while(*arg_str++ == '-');
					if(!get_token(&arg_str, last_token, MAX_CMD_ARG_NAME_LEN, &cur_state, &last_token_len)) {
						print("Bad syntax: argument names longer than ");
						print(itoa(MAX_CMD_ARG_NAME_LEN));
						print(" characters are not supported.\n");
						return 0;
					}
					break;
				case DOUBLE_QUOTE_STRING:
				case SINGLE_QUOTE_STRING:
					arg_str++; // skip first quote

					// check unclosed!

					if(last_state != PARAM_NAME) {
						if(args->unnamed_arg_count >= MAX_CMD_UNNAMED_ARG_COUNT) {
                                                        print("Bad syntax: commands containing more than ");
                                                        print(itoa(MAX_CMD_UNNAMED_ARG_COUNT));
                                                        print(" unnamed arguments are not supported.\n");
                                                        return 0;
                                                }

						if(!get_token(&arg_str, args->unnamed_args[args->unnamed_arg_count++], MAX_CMD_ARG_VALUE_LEN, &cur_state, &last_token_len)) {
                                                	print("Bad syntax: argument values longer than ");
                                                	print(itoa(MAX_CMD_ARG_NAME_LEN));
                                                	print(" characters are not supported.\n");
                                                	return 0;
                                        	}

						if(*cur_state == END_OF_INPUT) {
							println("Bad syntax: unterminated quoted string.");
							return 0;
						}
						break;
					}

					// explicit fallthrough - double and single quoted strings can be the values of named arguments
					// which should be handled the same way as plain old unquoted values, which the case below handles
				case PARAM_VALUE:
					if(cur_state == PARAM_VALUE)
						skip_ws(&arg_str); // skip whitespace if we aren't in a quoted string

					if(args->named_arg_count >= MAX_CMD_NAMED_ARG_COUNT) {
                                                print("Bad syntax: commands containing more than ");
                                                print(itoa(MAX_CMD_NAMED_ARG_COUNT));
                                                print(" named arguments are not supported.\n");
                                                return 0;
                                        } else {
                                                strcpy(args->named_arg_names[args->named_arg_count++], last_token);
                                        }

					if(!get_token(&arg_str, args->named_arg_values[args->named_arg_count++], MAX_CMD_ARG_VALUE_LEN, &cur_state, &last_token_len)) {
						print("Bad syntax: argument values longer than ");
						print(itoa(MAX_CMD_ARG_VALUE_LEN));
						print(" characters are not supported.\n");
						return 0;
					}

					if((last_state == DOUBLE_QUOTE_STRING || last_state == SINGLE_QUOTE_STRING) && cur_state == END_OF_INPUT) {
						println("Bad syntax: unterminated quoted string.");
						return 0;
					}
					break;
				case SINGLE_QUOTE_STRING_END_QUOTE:
				case DOUBLE_QUOTE_STRING_END_QUOTE:
					/* Skip the closing quote for strings and just adjust the last and current state, no need to consume a token */
					arg_str++;
					last_state = cur_state;
					changes_state(arg_str, cur_state, &cur_state);
					break;
				case DEFAULT:
					skip_ws(&arg_str);
					break;
				case END_OF_INPUT:
					if(last_state == PARAM_NAME) {
                                                if(args->flag_count >= MAX_CMD_FLAG_COUNT) {
                                                        print("Bad syntax: commands containing more than ");
                                                        print(itoa(MAX_CMD_FLAG_COUNT));
                                                        print(" flags are not supported.\n");
                                                        return 0;
                                                }

                                                strcpy(args->flags[args->flag_count++], last_token);
                                        }
					return 1;
			}
		}
	}
}

int get_token(char **arg_str, char *token, int max_token_len, enum SyntaxState *cur_state, int *token_len) {
	last_state = *cur_state;
	
	*token_len = 0;
	while(!changes_state(**arg_str, *cur_state, cur_state) && *token_len < max_token_len)
		token[*token_len++] = *(*arg_str++);
	token[*token_len] = '\0';
	
	if(*token_len >= max_token_len)
		return 0;
	else
		return 1;
}

/* Retrieves the value of a named argument from a string */
int arg(char *arg_str, char *arg_name, )
