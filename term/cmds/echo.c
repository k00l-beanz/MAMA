#include <lib/out.h>

int cmd_echo(char *arg_str) {
	(void)arg_str;

	println("hello", 5);
	return 0;
}
