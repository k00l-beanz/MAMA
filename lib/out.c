#include <modules/mpx_supt.h>

int print(char *str, int len) {
	return sys_req(WRITE, DEFAULT_DEVICE, str, &len);
}

int println(char *str, int len) {
	int ret = sys_req(WRITE, DEFAULT_DEVICE, str, &len);
	int one = 1;
	sys_req(WRITE, DEFAULT_DEVICE, "\n", &one);
	return ret;
}

int read(char *buf, int len) {
	return sys_req(READ, DEFAULT_DEVICE, buf, &len);
}
