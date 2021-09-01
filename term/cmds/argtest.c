#include "../args.h"
#include "../args.h"
#include "../args.c"
#include <lib/out.h>

int cmd_argtest(char *arg_str) {
	(void)arg_str;

	printf("This is a printf test with %i args, char: %c, string: %s, and a %% char!\n", 3, 'x', "abc", 1, 2, 3);

	parsed_args args = {0};
	if(parse_args(arg_str, &args)) {
		println("woo!", 1);

		println(args.flags[0], 1);
		printf("Command received %i flags:\n", args.flag_count);
		int i;
		for(i = 0; i < args.flag_count; i++)
			printf("- %s\n", args.flags[i]);

		printf("Command received %i named arguments:\n", args.named_arg_count);
		for(i = 0; i < args.named_arg_count; i++)
			printf("- %s = %s\n", args.named_arg_names[i], args.named_arg_values[i]);

		printf("Command received %i unnamed arguments:\n", args.unnamed_arg_count);
		for(i = 0; i < args.unnamed_arg_count; i++)
			printf("- %s\n", args.unnamed_args[i]);
	}

	return 0;
}
