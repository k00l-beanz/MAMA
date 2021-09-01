#include <lib/out.h>
#include "cursor.h"

/**
 * Writes a line of text under the user's prompt in the terminal. Recommended for providing hints or warnings to the user as they type.
 *
 * @param str The text to write under the user's prompt.
 * @param len The length of the text to write under the user's prompt.
 * @param ret_index The position to return the user's cursor to after writing the text.
 */
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
