#ifndef ARGS_H
#define ARGS_H

typedef struct parsed_args {
	int flag_count;
	int named_arg_count;
	int unnamed_arg_count;
	int unnamed_args_used_so_far;

	char flags[MAX_CMD_FLAG_COUNT][MAX_CMD_ARG_NAME_LEN + 1];
	char named_arg_names[MAX_CMD_NAMED_ARG_COUNT][MAX_CMD_ARG_NAME_LEN + 1];
	char named_arg_values[MAX_CMD_NAMED_ARG_COUNT][MAX_CMD_ARG_VALUE_LEN + 1];
	char unnamed_args[MAX_CMD_UNNAMED_ARG_COUNT][MAX_CMD_ARG_VALUE_LEN + 1];
} parsed_args;

parsed_args *parse_args(char *);

#endif
