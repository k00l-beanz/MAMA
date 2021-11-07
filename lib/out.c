#include <modules/mpx_supt.h>
#include <stdarg.h>

/*
 * Procedure: print()
 * Description: Write *str to serial output
*/
int print(char *str, int len) {
	return sys_req(WRITE, DEFAULT_DEVICE, str, &len);
}

/*
 * Procedure: printc()
 * Description: Write a single character to serial output
 */
int printc(char c) {
	char str[2];
	str[0] = c;
	str[1] = '\0';
	int one = 1;
	return sys_req(WRITE, DEFAULT_DEVICE, str, &one);	
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

/* Basic implementation of printf.
 * Will probably mess up if you don't give it the right number of arguments, so don't do that.
 */
void printf(char *str, ...) {
	va_list args;
	va_start(args, str);
	
	int len = strlen(str);
	char buf[len + 1];

	int i, j;
	for(i = 0, j = 0; str[i] != '\0'; i++) {
		if(str[i] != '%') {
			buf[j] = str[i];
			j++;
		} else {
			buf[j] = '\0';
			print(buf, j);
			j = 0;
			
			i++;
			
			switch(str[i]) {
				case 'i':
					print(itoa(va_arg(args, int)), 1);
					break;
				case 's':
					print(va_arg(args, char *), 1);
					break;
				case 'c':
					printc(va_arg(args, int));
					break;
				case '%':
					printc('%');
					break;
				default:
					printf("Illegal format string %%%c\n", str[i]);
					return;
			}
		}
	}
	buf[j] = '\0';
	print(buf, j);

	va_end(args);
}

/*
 * Procedure: read()
 * Description: Reads *buf into serial input.
*/
int read(char *buf, int len) {
	return sys_req(READ, DEFAULT_DEVICE, buf, &len);
}
