#include <include/string.h>

/**
 * Returns whether or not the specified character is a valid character in an identifier, such as a command or argument name.
 *
 * @param c The character to test.
 * @return True if the specified character c is valid in an identifier, false otherwise.
*/
int is_name_char(char c) {
	return  (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c >= '0' && c <= '9') ||
		(c == '_') || (c == '-');
}

/**
 * Moves the specified pointer to a character buffer forward until it points to the next non-whitespace character.
 *
 * @param c A pointer to a pointer to an entry in a character buffer. Will be modified to point to the next non-whitespace character in the buffer.
 */
void skip_ws(char **c) {
	while(isspace(*c))
			(*c)++;
}
