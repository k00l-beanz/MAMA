#include <core/io.h>
#include <core/tables.h>

#define BASE COM1

// some registers are used for more than one thing
#define DIVISOR_LATCH_LOW_BYTE_REGISTER 0
#define DIVISOR_LATCH_HIGH_BYTE_REGISTER 1
#define INTERRUPT_ENABLE_REGISTER 1
#define INTERRUPT_IDENTIFICATION_REGISTER 2
#define LINE_CONTROL_REGISTER 3
#define MODEM_CONTROL_REGISTER 4
#define LINE_STATUS_REGISTER 5
#define MODEM_STATUS_REGISTER 6
#define SCRATCH_REGISTER 7

#define PIC_MASK 0x21
#define PIC_END_OF_INTERRUPT_REGISTER 0x20
#define PIC_SIGNAL_END_OF_INTERRUPT_CODE 0x20

#define INTERRUPT_ID_OUTPUT_READY 0b01
#define INTERRUPT_ID_INPUT_READY 0b10

#define RING_BUFFER_SIZE 100

typedef enum {
	OPEN,

	CLOSED
} device_ready_state_t;

typedef enum {
	DEVICE_IDLE,

	DEVICE_READING,

	DEVICE_WRITING
} device_status_t;

typedef struct dcb_t {
	int *eflag_p;

	device_ready_state_t ready_state;

	device_status_t oper_status;

	char *user_read_buf;
	int *user_read_count;
	int user_read_progress;
	int user_read_index;

	char *user_write_buf;
	int *user_write_count;
	int user_write_progress;

	char ring_buffer[RING_BUFFER_SIZE];
	int ring_buffer_head;
	int ring_buffer_tail;
	int ring_buffer_cursor;
} dcb_t;

dcb_t *COM1_control_block = NULL;

int com_handle_interrupt();

int com_circular_next_index(int);
int com_circular_prev_index(int);

// return codes for all functions are defined by the r6 document

// TODO: disable interrupts at beginning, reenable at end?
int com_open(int *eflag_p, int baud_rate) {
	// 1.	Ensure that the parameters are valid, and that the device is not currently open.
	if(eflag_p == NULL) {
		return -101; // null event flag ptr
	}
	switch(baud_rate) {
		// supported baud rates: 110, 150, 300, 600, 1200, 2400, 4800, 9600, and 19,200
		// taken from max's glorious notes
		case 110:
		case 150:
		case 300:
		case 600:
		case 1200:
		case 2400:
		case 4800:
		case 9600:
		case 19200:
			break;
		default:
			return -102; // invalid baud rate divisor
	}
	if(COM1_control_block != NULL && COM1_control_block->ready_state != CLOSED) {
		return -103; // already open
	}

	// 2.	Initialize the DCB. In particular, this should include indicating that the device is open, saving a copy of the event flag pointer, and setting the initial device status to idle. In addition, the ring buffer parameters must be initialized.
	if(COM1_control_block == NULL) {
		COM1_control_block = sys_alloc_mem(sizeof(dcb_t));
	}
	COM1_control_block->eflag_p = eflag_p;
	COM1_control_block->ready_state = OPEN;
	COM1_control_block->oper_status = DEVICE_IDLE;
	COM1_control_block->user_read_buf = NULL;
	COM1_control_block->user_read_count = NULL;
	COM1_control_block->user_read_progress = 0;
	COM1_control_block->user_read_index = 0;
	COM1_control_block->user_write_buf = NULL;
	COM1_control_block->user_write_count = NULL;
	COM1_control_block->user_write_progress = 0;
	COM1_control_block->ring_buffer_head = 0;
	COM1_control_block->ring_buffer_tail = 0;
	COM1_control_block->ring_buffer_cursor = 0;

	// 3.	Save the address of the current interrupt handler, and install the new handler in the interrupt vector.
	// TODO: save current address part
	cli();
	idt_set_gate(0x24, (u32int)com_handle_interrupt, 0x08, 0x8e);

	// 4.	Compute the required baud rate divisor.
	int baud_rate_divisor = 115200 / (long) baud_rate;

	// 5.	Store the value 0x80 in the Line Control Register. This allows the first two port addresses to access the Baud Rate Divisor register.
	outb(BASE + LINE_CONTROL_REGISTER, 0x80);

	// 6.	Store the high order and low order bytes of the baud rate divisor into the MSB and LSB registers, respectively.
	outb(BASE + DIVISOR_LATCH_LOW_BYTE_REGISTER, baud_rate_divisor & 0x00ff);
	outb(BASE + DIVISOR_LATCH_HIGH_BYTE_REGISTER, (baud_rate_divisor & 0xff00) >> 8); // is this how you calculate "high order and low order bytes" i dont know lol

	// 7.	Store the value 0x03 in the Line Control Register. This sets the line characteristics to 8 data bits, 1 stop bit, and no parity. It also restores normal functioning of the first two ports. 
	outb(BASE + LINE_CONTROL_REGISTER, 0x03);

	//outb(BASE + 2, 0b11000111);

	// 8.	Enable the appropriate level in the PIC mask register.
	outb(PIC_MASK, inb(PIC_MASK) & ~0b10000); // PIC level for COM1 is 4, not sure if this is the right way to set it or not though

	// 9.	Enable overall serial port interrupts by storing the value 0x08 in the Modem Control register. 
	outb(BASE + MODEM_CONTROL_REGISTER, 0x08);

	// 10.	Enable input ready interrupts only by storing the value 0x01 in the Interrupt Enable register.
	outb(BASE + INTERRUPT_ENABLE_REGISTER, 0x01);

	inb(BASE);
	sti();

	return 0;
}

int com_close() {
	// 1.	Ensure that the port is currently open.
	if(COM1_control_block == NULL || COM1_control_block->ready_state != OPEN) {
		return -201; // not open
	}

	// 2.	Clear the open indicator in the DCB.
	COM1_control_block->ready_state = CLOSED;

	// 3.	Disable the appropriate level in the PIC mask register. 
	// TODO

	// 4.	Disable all interrupts in the ACC by loading zero values to the Modem Status register and the Interrupt Enable register.
	outb(BASE + MODEM_STATUS_REGISTER, 0x0);
	outb(BASE + INTERRUPT_ENABLE_REGISTER, 0x0);

	// 5.	Restore the original saved interrupt vector. 
	// TODO

	return 0;
}

int com_read(char *buf, int *count) {
	// 1.	Validate the supplied parameters.
	if(buf == NULL) {
		return -302; // invalid buffer address
	}
	if(count == NULL || *count <= 0) {
		return -303; // invalid count address or count value
	}

	// 2.	Ensure that the port is open, and the status is idle.
	if(COM1_control_block == NULL || COM1_control_block->ready_state != OPEN) {
		return -301; // port not open
	}
	if(COM1_control_block->oper_status != DEVICE_IDLE) {
		return -304; // device busy
	}

	// 3.	Initialize the input buffer variables (not the ring buffer!) and set the status to reading. 
	int actual_count = 0;
	COM1_control_block->user_read_count = count;
	COM1_control_block->user_read_buf = buf;
	COM1_control_block->user_read_index = 0;
	COM1_control_block->user_read_progress = 0;
	COM1_control_block->oper_status = DEVICE_READING;

	// 4.	Clear the caller's event flag. 
	*(COM1_control_block->eflag_p) = 0;

	// 5.	Copy characters from the ring buffer to the requestor's buffer, until the ring buffer is emptied, the requested count has been reached, or a CR (ENTER) code has been found. The copied characters should, of course, be removed from the ring buffer. Either input interrupts or all interrupts should be disabled during the copying. 
	int prev_interrupt_enable_reg_value = inb(BASE + INTERRUPT_ENABLE_REGISTER);
	outb(BASE + INTERRUPT_ENABLE_REGISTER, 0x0); // disable input interrupts TODO: this might clear the write interrupt bit and not reset it


	while(actual_count < *count && COM1_control_block->ring_buffer_head != COM1_control_block->ring_buffer_tail) {
		serial_println("using from ring buffer first...");
		char next = COM1_control_block->ring_buffer[COM1_control_block->ring_buffer_head];
		COM1_control_block->ring_buffer[COM1_control_block->ring_buffer_head] = '\0'; // I don't think this is actually necessary, but it says to do it
		COM1_control_block->ring_buffer_head = com_circular_next_index(COM1_control_block->ring_buffer_head);

		if(next == '\n')
			break;

		buf[actual_count] = next;
		actual_count++;
	}

	outb(BASE + INTERRUPT_ENABLE_REGISTER, prev_interrupt_enable_reg_value); // reenable input interrupts

	// 6.	If more characters are needed, return. If the block is complete, continue with step 7. 
	if(actual_count < *count) {
		//serial_println("didnt get everything from ring buffer");
		return 0;
	}
	//serial_println("got everything from ring buffer");

	// 7.	Reset the DCB status to idle, set the event flag, and return the actual count to the requestor's variable. 
	COM1_control_block->oper_status = DEVICE_IDLE;
	*(COM1_control_block->eflag_p) = 1; // again, not sure what it wants here but i think its this
	*count = actual_count;
	COM1_control_block->user_read_buf[actual_count] = '\0';

	return 0;
}

int com_write(char *buf, int *count) {
	// 1.	Ensure that the input parameters are valid.
	if(buf == NULL) {
		return -402; // invalid buffer address
	}
	if(count == NULL || *count <= 0) {
		return -403; // invalid count address or count value
	}

	// 2.	Ensure that the port is currently open and idle. 
	if(COM1_control_block == NULL || COM1_control_block->ready_state != OPEN) {
		return -401; // serial port not open
	}
	if(COM1_control_block->oper_status != DEVICE_IDLE) {
		return -404; // device busy
	}

	// 3.	Install the buffer pointer and counters in the DCB, and set the current status to writing. 
	COM1_control_block->user_write_buf = buf;
	COM1_control_block->user_write_count = count;
	COM1_control_block->user_write_progress = 0;
	COM1_control_block->oper_status = DEVICE_WRITING;

	// 4.	Clear the caller's event flag. 
	*(COM1_control_block->eflag_p) = 0;

	// 5.	Get the first character from the requestor's buffer and store it in the output register. 
	outb(BASE, buf[0]);

	COM1_control_block->user_write_progress++;

	// 6.	Enable write interrupts by setting bit 1 of the Interrupt Enable register. This must be done by setting the register to the logical or of its previous contents and 0x02.
	outb(BASE + INTERRUPT_ENABLE_REGISTER, inb(BASE + INTERRUPT_ENABLE_REGISTER) | 0x02);

	return 0;
}


// -------------------------- interrupt handlers --------------------------

#include <term/history.h>
#include <term/visuals/syntax_highlight.h>
#include <term/visuals/syntax_highlight.c>

#define DELETE      0b00001
#define LEFT_ARROW  0b00010
#define RIGHT_ARROW 0b00100
#define UP_ARROW    0b01000
#define DOWN_ARROW  0b10000

// TODO: this still busy waits
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

int com_handle_interrupt_read() {
	if(COM1_control_block == NULL) {
		return 1;
	}
	if(COM1_control_block->oper_status != DEVICE_READING) {
		serial_println("sending to ring_buffer");
		char letter = inb(BASE);
		switch (letter) {
				/*Backspace Key */
				case '\b':
				case 0x7f:
					/* 
					 *	Move index + chars_read back ONLY IF we are not at the
					 *	start of the line (don't let the user backspace off the screen)
					 *	this case would also be a buffer underflow.
					 */
					if (COM1_control_block->ring_buffer_head != COM1_control_block->ring_buffer_cursor) {
						for (int i = com_circular_prev_index(COM1_control_block->ring_buffer_cursor); com_circular_next_index(i) != COM1_control_block->ring_buffer_tail; i = com_circular_next_index(i))
							COM1_control_block->ring_buffer[i] = COM1_control_block->ring_buffer[com_circular_next_index(i)];
						COM1_control_block->ring_buffer_cursor = com_circular_prev_index(COM1_control_block->ring_buffer_cursor);
						COM1_control_block->ring_buffer_tail = com_circular_prev_index(COM1_control_block->ring_buffer_tail);

						// adjust visually
						outb(COM1, '\b');
						for (int i = COM1_control_block->ring_buffer_cursor; i != COM1_control_block->ring_buffer_tail; i = com_circular_next_index(i)) {
							//syntax_handle_char(buffer[i], i);
							outb(COM1, COM1_control_block->ring_buffer[i]);
						}
						outb(COM1, ' ');
						outb(COM1, '\b');
						for (int i = COM1_control_block->ring_buffer_cursor; i != COM1_control_block->ring_buffer_tail; i = com_circular_next_index(i))
							outb(COM1, '\b');
					}
					break;
				/* Everything else */
				default:
					// anything else just gets added to buffer and printed
					if(COM1_control_block->ring_buffer_head != com_circular_prev_index(COM1_control_block->ring_buffer_tail)) {
						for (int i = COM1_control_block->ring_buffer_tail; i != COM1_control_block->ring_buffer_cursor; i = com_circular_prev_index(i))
							COM1_control_block->ring_buffer[i] = COM1_control_block->ring_buffer[com_circular_prev_index(i)];
						COM1_control_block->ring_buffer[COM1_control_block->ring_buffer_cursor] = letter;
						//syntax_handle_char(letter, index);
						COM1_control_block->ring_buffer_tail = com_circular_next_index(COM1_control_block->ring_buffer_tail);
						COM1_control_block->ring_buffer_cursor = com_circular_next_index(COM1_control_block->ring_buffer_cursor);
						outb(COM1, letter);
						for (int i = COM1_control_block->ring_buffer_cursor; i < COM1_control_block->ring_buffer_tail; i = com_circular_next_index(i)) {
							//syntax_handle_char(buffer[i], i);
							outb(COM1, COM1_control_block->ring_buffer[i]);
						}
						for (int i = COM1_control_block->ring_buffer_cursor; i < COM1_control_block->ring_buffer_tail; i = com_circular_next_index(i)) {
							outb(COM1, '\b');
						}
					}
			}
		return 0;
	}

	char letter = inb(COM1); /* Holds individual letter */
	int i;
	int chars_read = COM1_control_block->user_read_progress;
	char *buffer = COM1_control_block->user_read_buf;
	int index = COM1_control_block->user_read_index;
	int got_newline = 0;
	switch (letter) {
		/* Return Key */
		case '\r':
		case '\n':
			/* 
			 *	Carriage Return (CR) and Line Feed (LF) count as newlines or 'end of input' chars.
			 *	Null terminate and return, don't add newline to buffer. 
			 */
			buffer[chars_read] = '\0';
			got_newline = 1;
			break;
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
					//syntax_handle_char(buffer[i], i);
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
							//syntax_handle_char(buffer[i], i);
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
					//hist_rewind(buffer, &index, &chars_read);
					break;
				case DOWN_ARROW:
					//hist_forward(buffer, &index, &chars_read);
					break;
			}
			break;
		/* Everything else */
		default:
			// anything else just gets added to buffer and printed
			//for (i = chars_read; i > index; i--)
			//	buffer[i] = buffer[i - 1];
			//buffer[index] = letter;
			//syntax_handle_char(letter, index);
			outb(COM1, letter);
			chars_read += 1;
			index += 1;
			for (i = index; i < chars_read; i++) {
				//syntax_handle_char(buffer[i], i);
				outb(COM1, buffer[i]);
			}
			for (i = index; i < chars_read; i++) {
				outb(COM1, '\b');
			}
	}

	COM1_control_block->user_read_progress = chars_read;
	COM1_control_block->user_read_index = index;

	if(got_newline || COM1_control_block->user_read_progress >= *(COM1_control_block->user_read_count)) {
		serial_println("here");
		*(COM1_control_block->user_read_count) = COM1_control_block->user_read_progress;
		COM1_control_block->oper_status = DEVICE_IDLE;
		*(COM1_control_block->eflag_p) = 1;
	}

	return 0;
}

int com_handle_interrupt_write() {
	if(COM1_control_block == NULL || COM1_control_block->oper_status != DEVICE_WRITING) {
		return 1;
	}

	if(COM1_control_block->user_write_progress < *(COM1_control_block->user_write_count)) {
		// not done
		outb(BASE, COM1_control_block->user_write_buf[COM1_control_block->user_write_progress]);
		COM1_control_block->user_write_progress++;
	} else {
		// done
		//serial_println("X");
		*(COM1_control_block->eflag_p) = 1;
		COM1_control_block->oper_status = DEVICE_IDLE;
		outb(BASE + INTERRUPT_ENABLE_REGISTER, inb(BASE + INTERRUPT_ENABLE_REGISTER) & ~0x02);
	}

	return 0;
}

int com_handle_interrupt() {
	//serial_println("top of com_handle_interrupt");
	if(COM1_control_block == NULL || COM1_control_block->ready_state != OPEN) {
		return 1;
	}
	//serial_println("x");
	//cli();
	int interrupt_id = inb(BASE + INTERRUPT_IDENTIFICATION_REGISTER);
	
	// was caused by serial port? (bit 0 == 0?)
	if((interrupt_id & 0b1)) {
		return 1;
	}

	if((interrupt_id & 0b0110) >> 1 == INTERRUPT_ID_OUTPUT_READY) {
		com_handle_interrupt_write();
	}
	else if((interrupt_id & 0b0110) >> 1 == INTERRUPT_ID_INPUT_READY) {
		//serial_println("got read interrupt");
		//if(COM1_control_block->oper_status == DEVICE_READING)
		//	serial_println("something is reading");
		//else
		//	serial_println("something is not reading");
		outb(BASE, inb(BASE));
		//inb(BASE);
		//com_handle_interrupt_read();
	}

	outb(PIC_END_OF_INTERRUPT_REGISTER, PIC_SIGNAL_END_OF_INTERRUPT_CODE);
	//sti();
	//serial_println("bottom of com_handle_interrupt");
	return 0;
}

int com_circular_next_index(int i) {
	return i == RING_BUFFER_SIZE - 1 ? 0 : i + 1;
}

int com_circular_prev_index(int i) {
	return i == 0 ? RING_BUFFER_SIZE - 1 : i - 1;
}