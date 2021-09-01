#include "commhand.h"
#include "utils.h"
#include "args.h"
#include "syntax.h"
#include <lib/out.h>
#include <include/string.h>

/* Currently, the only states that can go on the parse stack are PARAM_NAME, DOUBLE_QUOTE_STRING, and SINGLE_QUOTE_STRING.
 * At most, a stack can contain one PARAM_NAME and either DOUBLE_QUOTE_STRING or SINGLE_QUOTE_STRING, which means that
 * 2 is a suitable max stack size for now.
 */
#define MAX_PARSE_STACK_SIZE 2
enum SyntaxState parse_stack[MAX_PARSE_STACK_SIZE];
int stack_size = 0;

enum SyntaxState last_state;
enum SyntaxState cur_state;

int get_token(char **, char *, int);
int stack_empty();
enum SyntaxState stack_peek();
void stack_push(enum SyntaxState);
void stack_pop();

int parse_args(char *arg_str, parsed_args *args) {
	skip_ws(&arg_str);
	
	// The only lexeme we have to store and use a lookahead to figure out what to do with is PARAM_NAME,
	// which could either be a flag or the name of a named argument depending on what comes next.
	// Therefore, allocating a buffer with enough room to store argument names is sufficient here, because
	// no other type of lexeme will be written to this buffer.
	char last_token[MAX_CMD_ARG_NAME_LEN + 1];
	
	last_state = DEFAULT;
	cur_state = last_state;
	
	int running = 1;
	while(running) {
		print("arg_str is pointing to ", 1);
		printc(*arg_str);
		printc('\n');
		switch(cur_state) {
			case PARAM_NAME:
				println("Switched to state PARAM_NAME", 1);
				if(stack_peek() == PARAM_NAME) {
					if(args->flag_count >= MAX_CMD_FLAG_COUNT) {
						print("Bad syntax: commands containing more than ", 1);
						print(itoa(MAX_CMD_FLAG_COUNT), 1);
						print(" flags are not supported.\n", 1);
						return 0;
					}
					
					strcpy(args->flags[args->flag_count++], last_token);
				} else {
					stack_push(PARAM_NAME);
				}

				while(*arg_str == '-')
					arg_str++;
				if(!get_token(&arg_str, last_token, MAX_CMD_ARG_NAME_LEN)) {
					print("Bad syntax: argument names longer than ", 1);
					print(itoa(MAX_CMD_ARG_NAME_LEN), 1);
					print(" characters are not supported.\n", 1);
					return 0;
				}

				break;
			case DOUBLE_QUOTE_STRING:
			case SINGLE_QUOTE_STRING:
				println("Switched to state STRING", 1);
				arg_str++; // skip first quote

				if(stack_peek() == PARAM_NAME) {
					if(!get_token(&arg_str, args->named_arg_values[args->named_arg_count++], MAX_CMD_ARG_VALUE_LEN)) {
						print("Bad syntax: argument values longer than ", 1);
						print(itoa(MAX_CMD_ARG_VALUE_LEN), 1);
						print(" characters are not supported.\n", 1);
						return 0;
					}
				} else {
					if(args->unnamed_arg_count >= MAX_CMD_UNNAMED_ARG_COUNT) {
						print("Bad syntax: commands containing more than ", 1);
						print(itoa(MAX_CMD_UNNAMED_ARG_COUNT), 1);
						print(" unnamed arguments are not supported.\n", 1);
						return 0;
					}
					
					if(!get_token(&arg_str, args->unnamed_args[args->unnamed_arg_count++], MAX_CMD_ARG_VALUE_LEN)) {
						print("Bad syntax: argument values longer than ", 1);
						print(itoa(MAX_CMD_ARG_VALUE_LEN), 1);
						print(" characters are not supported.\n", 1);
						return 0;
					}
				}

				stack_push(cur_state);
				break;
			case PARAM_VALUE:
				println("Switched to state PARAM_VALUE", 1);
				if(stack_peek() == PARAM_NAME) {
					if(args->named_arg_count >= MAX_CMD_NAMED_ARG_COUNT) {
						print("Bad syntax: commands containing more than ", 1);
						print(itoa(MAX_CMD_NAMED_ARG_COUNT), 1);
						print(" named arguments are not supported.\n", 1);
						return 0;
					} else {
						strcpy(args->named_arg_names[args->named_arg_count++], last_token);
					}

					if(!get_token(&arg_str, args->named_arg_values[args->named_arg_count], MAX_CMD_ARG_VALUE_LEN)) {
						print("Bad syntax: argument values longer than ", 1);
						print(itoa(MAX_CMD_ARG_VALUE_LEN), 1);
						print(" characters are not supported.\n", 1);
						return 0;
					}
					
					stack_pop();
				} else {
					if(args->unnamed_arg_count >= MAX_CMD_UNNAMED_ARG_COUNT) {
						print("Bad syntax: commands containing more than ", 1);
						print(itoa(MAX_CMD_UNNAMED_ARG_COUNT), 1);
						print(" unnamed arguments are not supported.\n", 1);
						return 0;
					}
					
					if(!get_token(&arg_str, args->unnamed_args[args->unnamed_arg_count++], MAX_CMD_ARG_VALUE_LEN)) {
						print("Bad syntax: argument values longer than ", 1);
						print(itoa(MAX_CMD_ARG_VALUE_LEN), 1);
						print(" characters are not supported.\n", 1);
						return 0;
					}
				}

				break;
			case SINGLE_QUOTE_STRING_END_QUOTE:
			case DOUBLE_QUOTE_STRING_END_QUOTE:
				println("Switched to state STRING_END_QUOTE", 1);
				/* Skip the closing quote for strings and just adjust the last and current state, no need to consume a token */
				stack_pop();
				if(stack_peek() == PARAM_NAME) {
					strcpy(args->named_arg_names[args->named_arg_count], last_token);
					stack_pop();
				}
				arg_str++;
				last_state = cur_state;
				cur_state = get_state(*arg_str, cur_state);
				break;
			case DEFAULT:
				println("Switched to state DEFAULT", 1);
				skip_ws(&arg_str);
				last_state = DEFAULT;
				cur_state = get_state(*arg_str, cur_state);
				break;
			case END_OF_INPUT:
				println("Switched to state END_OF_INPUT", 1);
				if(stack_peek() == DOUBLE_QUOTE_STRING || stack_peek() == SINGLE_QUOTE_STRING) {
					println("Bad syntax: unterminated quoted string.", 1);
					return 0;
				} else if(stack_peek() == PARAM_NAME) {
					if(args->flag_count >= MAX_CMD_FLAG_COUNT) {
						print("Bad syntax: commands containing more than ", 1);
						print(itoa(MAX_CMD_FLAG_COUNT), 1);
						print(" flags are not supported.\n", 1);
						return 0;
					}

					strcpy(args->flags[args->flag_count++], last_token);
				}
				running = 0;
				break;
			case CMD_NAME:
			case CMD_NAME_OR_LEADING_WHITESPACE:
			default:
				/* This should never happen, but for completeness and to avoid compile errors is included */
				println("Bad syntax: unexpected lexeme or state in arguments string", 1);
				return 0;
		}
	}
	return 1;
}


int get_token(char **arg_str, char *token, int max_token_len) {
	last_state = cur_state;
	
	int i = 0;
	while(!changes_state(**arg_str, cur_state, &cur_state) && i < max_token_len) {
		print("Character ", 1);
		printc(**arg_str);
		println(" didn't change state, writing to token buf", 1);
		token[i++] = *((*arg_str)++);
	}
	token[i] = '\0';
	
	print("Consumed token ", 1);
	println(token, 1);
	
	if(i >= max_token_len)
		return 0;
	else
		return 1;
}

int stack_empty() {
	return stack_size == 0;
}

enum SyntaxState stack_peek() {
	return stack_empty() ? NULL : parse_stack[stack_size - 1];
}

void stack_push(enum SyntaxState state) {
	parse_stack[stack_size++] = state;
}

void stack_pop() {
	stack_size--;
}

/* Retrieves the value of a named argument */
int named_arg(parsed_args *args, char *arg_name, char **arg_val) {
	int i;
	for(i = 0; i < args->named_arg_count; i++) {
		if(strcmp(arg_name, args->named_arg_names[i]) == 0) {
			*arg_val = args->named_arg_values[i];
			return 1;
		}
	}
	return 0;
}

/* Returns true if the specified flag is set, false otherwise */
int flag(parsed_args *args, char *flag_name) {
	int i;
	for(i = 0; i < args->flag_count; i++) {
		if(strcmp(flag_name, args->flags[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

/* Retrieves the next unnamed argument */
int next_unnamed_arg(parsed_args *args, char **arg_val) {
	if(args->unnamed_args_used_so_far < args->unnamed_arg_count) {
		*arg_val = args->unnamed_args[args->unnamed_args_used_so_far++];
		return 1;
	}
	return 0;
}
