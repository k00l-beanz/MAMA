#include <lib/out.h>
#include "cursor.h"

void hint_under_prompt(char *str, int len, int ret_index) {
	cursor_down(1);
	cursor_return();
	print(str, len);
	cursor_up(1);
	if(len > ret_index)
		cursor_left(len - ret_index);
	else
		cursor_right(ret_index - len);
}
