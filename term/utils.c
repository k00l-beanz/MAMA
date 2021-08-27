/*
 * Procedure: is_name_char
 * Description: Checks for invalid characters
*/
int is_name_char(char c) {
	return  (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c >= '0' && c <= '9') ||
		(c == '_');
}