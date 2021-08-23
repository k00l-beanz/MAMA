#include <modules/mpx_supt.h>

/*
 * Procedure: print()
 * Description: Write *str to serial output
*/
int print(char *str, int len) {
	return sys_req(WRITE, DEFAULT_DEVICE, str, &len);
}

/*
 * Procedure: println()
 * Description: Writes *str to serial output. Appends newline
 *  to the end.
*/
int println(char *str, int len) {
	int ret = sys_req(WRITE, DEFAULT_DEVICE, str, &len);
	int one = 1;
	sys_req(WRITE, DEFAULT_DEVICE, "\n", &one);
	return ret;
}

/*
 * Procedure: read()
 * Description: Reads *buf into serial input.
*/
int read(char *buf, int len) {
	return sys_req(READ, DEFAULT_DEVICE, buf, &len);
}
