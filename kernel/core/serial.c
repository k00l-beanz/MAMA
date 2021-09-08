/*
	----- serial.c -----

	Description..: Contains methods and variables used for
	serial input and output.
*/
#include <stdint.h>
#include <string.h>
#include <core/io.h>
#include <core/serial.h>
#define NO_ERROR 0

// Active devices used for serial I/O
int serial_port_out = 0;
int serial_port_in = 0;

/*
	Procedure..: init_serial
	Description..: Initializes devices for user interaction, logging, ...
*/
int init_serial(int device) {
	outb(device + 1, 0x00);	// disable interrupts
	outb(device + 3, 0x80);	// set line control register
	outb(device + 0, 115200 / 9600);	// set bsd least sig bit
	outb(device + 1, 0x00);	// brd most significant bit
	outb(device + 3, 0x03);	// lock divisor; 8bits, no parity, one stop
	outb(device + 2, 0xC7);	// enable fifo, clear, 14byte threshold
	outb(device + 4, 0x0B);	// enable interrupts, rts/dsr set
	(void) inb(device);	// read bit to reset port
	return NO_ERROR;
}

/*
  Procedure..: serial_println
  Description..: Writes a message to the active serial output device.
    Appends a newline character.
*/
int serial_println(const char *msg) {
	int i;
	for (i = 0; *(i + msg) != '\0'; i++) {
		outb(serial_port_out, *(i + msg));
	}
	outb(serial_port_out, '\r');
	outb(serial_port_out, '\n');
	return NO_ERROR;
}

/*
	Procedure..: serial_print
	Description..: Writes a message to the active serial output device.
*/
int serial_print(const char *msg) {
	int i;
	for (i = 0; *(i + msg) != '\0'; i++) {
		outb(serial_port_out, *(i + msg));
	}
	if (*msg == '\r')
		outb(serial_port_out, '\n');
	return NO_ERROR;
}

/*int (*student_read)(char *buffer, int *count);
	Procedure..: set_serial_out
	Description..: Sets serial_port_out to the given device address.
	All serial output, such as that from serial_println, will be
	directed to this device.
*/
int set_serial_out(int device) {
	serial_port_out = device;
	return NO_ERROR;
}

/*
	Procedure..: set_serial_in
	Description..: Sets serial_port_in to the given device address.
	All serial input, such as console input via a virtual machine,
	QEMU/Bochs/etc, will be directed to this device.
*/
int set_serial_in(int device) {
	serial_port_in = device;
	return NO_ERROR;
}

#include <term/history.h>
#include <term/visuals/syntax_highlight.h>
#include <term/visuals/syntax_highlight.c>

#define DELETE      0b00001
#define LEFT_ARROW  0b00010
#define RIGHT_ARROW 0b00100
#define UP_ARROW    0b01000
#define DOWN_ARROW  0b10000

unsigned int consume_special();

// I made a mess of ur polling function I apologize 
/* WTF is this Austin!?! Unreadable! jk good job -Maximillian */

/**
 * Serially poll characters from command line
 * 
 * Polls input from keyboard and interprets each character individually
 * as it is entered from the keyboard.
 * 
 * @param buffer Space allocated for single line on the command line
 * @param count Size of the space allocated
 * 
 * @return Returns 0 upon success, -1 upon error
 * 
*/
int *polling(char *buffer, int *count) {
	int chars_read = 0;
	int index = 0;

	/* Driver loop */
	while (chars_read < *count) {
		if (inb(COM1 + 5) &1) { /* If there is incoming data */
			char letter = inb(COM1); /* Holds individual letter */
			int i;
			switch (letter) {
				/* Return Key */
				case '\r':
				case '\n':
					/* 
					 *	Carriage Return (CR) and Line Feed (LF) count as newlines or 'end of input' chars.
					 *	Null terminate and return, don't add newline to buffer. 
					 */
					buffer[chars_read] = '\0';
					return NO_ERROR;
				/*Backspace Key */
				case '\b':
				case 0x7f:
					/* 
					 *	Move index + chars_read back ONLY IF we are not at the
					 *	start of the line (don't let the user backspace off the screen)
					 *	this case would also be a buffer underflow.
					 */
					if (index > 0) {
						for (i = index; i < chars_read; i++)
							buffer[i - 1] = buffer[i];
						index--;
						chars_read--;

						// adjust visually
						outb(COM1, '\b');
						for (i = index; i < chars_read; i++) {
							syntax_handle_char(buffer[i], i);
							outb(COM1, buffer[i]);
						}
						outb(COM1, ' ');
						outb(COM1, '\b');
						for (i = index; i < chars_read; i++)
							outb(COM1, '\b');
					}
					break;
				/* Special Characters */
				case '\e':
					switch (consume_special()) {
						/* Delete Key */
						case DELETE:
							/*
							 *	Shift all characters in front of the current index back 1 space
							 *	only attempt to do so if we aren't at the end of the line
							 */
							if (index < chars_read) {
								for (i = index; i < chars_read - 1; i++)
									buffer[i] = buffer[i + 1];
								chars_read--;

								/* adjust visually */
								for (i = index; i < chars_read; i++) {
									syntax_handle_char(buffer[i], i);
									outb(COM1, buffer[i]);
								}
								outb(COM1, ' ');
								outb(COM1, '\b');
								for (i = index; i < chars_read; i++)
									outb(COM1, '\b');
							}
							break;
						/* Left Arrow */
						case LEFT_ARROW:
							/* Move index to the left if it isn't at the start of the line */
							if (index > 0) {
								index--;
								outb(COM1, '\b');
							}
							break;
						/* Right Arrow */
						case RIGHT_ARROW:
							/* Move index to the right if it isn't at the end of the line */
							if (index < chars_read) {
								index++;
								outb(COM1, '\e');
								outb(COM1, '[');
								outb(COM1, 'C');
							}
							break;
						/* History with Up and Down arrow keys */
						case UP_ARROW:
							hist_rewind(buffer, &index, &chars_read);
							break;
						case DOWN_ARROW:
							hist_forward(buffer, &index, &chars_read);
							break;
					}
					break;
				/* Everything else */
				default:
					// anything else just gets added to buffer and printed
					for (i = chars_read; i > index; i--)
						buffer[i] = buffer[i - 1];
					buffer[index] = letter;
					syntax_handle_char(letter, index);
					outb(COM1, letter);
					chars_read += 1;
					index += 1;
					for (i = index; i < chars_read; i++) {
						syntax_handle_char(buffer[i], i);
						outb(COM1, buffer[i]);
					}
					for (i = index; i < chars_read; i++)
						outb(COM1, '\b');
			}
		}
	}

	return NO_ERROR;
}

unsigned int consume_special() {
	unsigned int possibilities = 0b11111;

	int i = 1;
	while (possibilities != 0) {
		if (inb(COM1 + 5) &1) {
			char c = inb(COM1);

			switch (i) {
				case 1:
					if (c != 0x5b) possibilities &= ~DELETE & ~LEFT_ARROW & ~RIGHT_ARROW & ~UP_ARROW & ~DOWN_ARROW;
					break;
				case 2:
					if (c != 0x33) possibilities &= ~DELETE;

					if (c == 'A' && possibilities & UP_ARROW)    return UP_ARROW;
					if (c == 'B' && possibilities & DOWN_ARROW)  return DOWN_ARROW;
					if (c == 'C' && possibilities & RIGHT_ARROW) return RIGHT_ARROW;
					if (c == 'D' && possibilities & LEFT_ARROW)  return LEFT_ARROW;

					possibilities &= ~UP_ARROW & ~DOWN_ARROW & ~RIGHT_ARROW & ~LEFT_ARROW;
					break;
				case 3:
					if (c == 0x7e && possibilities == DELETE) return DELETE;

					possibilities &= ~DELETE;
					break;
				default:
					possibilities = 0;
			}

			i++;
		}
	}

	return 0;
}
