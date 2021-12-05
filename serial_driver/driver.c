// TODO: import the right stuff

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

#define RING_BUFFER_SIZE 100

typedef enum {
	OPEN,

	CLOSED
} device_ready_state_t;

typedef enum {
	IDLE,

	READING,

	WRITING
} device_status_t;

typedef struct dcb_t {
	int *eflag_p;

	device_ready_state_t ready_state;

	device_status_t oper_status;
} dcb_t;

const dcb_t *COM1_control_block = NULL;

char ring_buffer[RING_BUFFER_SIZE];
int ring_buffer_head = 0;
int ring_buffer_tail = 0;

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
		case 600:   // i dont think this is how youre supposed to program
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
	COM1_control_block->oper_status = IDLE;
	ring_buffer_head = 0;
	ring_buffer_tail = 0;

	// 3.	Save the address of the current interrupt handler, and install the new handler in the interrupt vector.
	// TODO: this ^^^

	// 4.	Compute the required baud rate divisor.
	int baud_rate_divisor = 115200 / (long) baud_rate;

	// 5.	Store the value 0x80 in the Line Control Register. This allows the first two port addresses to access the Baud Rate Divisor register.
	outb(BASE + LINE_CONTROL_REGISTER, 0x80);

	// 6.	Store the high order and low order bytes of the baud rate divisor into the MSB and LSB registers, respectively.
	outb(BASE + DIVISOR_LATCH_LOW_BYTE_REGISTER, baud_rate_divisor & 0x00ff);
	outb(BASE + DIVISOR_LATCH_HIGH_BYTE_REGISTER, baud_rate_divisor & 0xff00); // is this how you calculate "high order and low order bytes" i dont know lol

	// 7.	Store the value 0x03 in the Line Control Register. This sets the line characteristics to 8 data bits, 1 stop bit, and no parity. It also restores normal functioning of the first two ports. 
	outb(BASE + LINE_CONTROL_REGISTER, 0x03);

	// 8.	Enable the appropriate level in the PIC mask register.
	// lol what

	// 9.	Enable overall serial port interrupts by storing the value 0x08 in the Modem Control register. 
	outb(BASE + MODEM_CONTROL_REGISTER, 0x08);

	// 10.	Enable input ready interrupts only by storing the value 0x01 in the Interrupt Enable register.
	outb(BASE + INTERRUPT_ENABLE_REGISTER, 0x01);

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
	if(COM1_control_block->oper_status != IDLE) {
		return -304; // device busy
	}

	// 3.	Initialize the input buffer variables (not the ring buffer!) and set the status to reading. 
	int actual_count = 0;
	COM1_control_block->oper_status = READING;

	// 4.	Clear the caller's event flag. 
	*(COM1_control_block->eflag_p) = 0; // is this what it means?

	// 5.	Copy characters from the ring buffer to the requestor's buffer, until the ring buffer is emptied, the requested count has been reached, or a CR (ENTER) code has been found. The copied characters should, of course, be removed from the ring buffer. Either input interrupts or all interrupts should be disabled during the copying. 
	outb(BASE + INTERRUPT_ENABLE_REGISTER, 0x0); // disable input interrupts

	while(actual_count < *count && ring_buffer_head != ring_buffer_tail) {
		char next = ring_buffer[ring_buffer_head];
		ring_buffer[ring_buffer_head] = '\0'; // I don't think this is actually necessary, but it says to do it
		ring_buffer_head = ring_buffer_head == RING_BUFFER_SIZE - 1 ? 0 : ring_buffer_head + 1;

		if(next == '\n')
			break;

		actual_count++;
	}

	outb(BASE + INTERRUPT_ENABLE_REGISTER, 0x1); // reenable input interrupts

	// 6.	If more characters are needed, return. If the block is complete, continue with step 7. 
	if(actual_count < *count)
		return;

	// 7.	Reset the DCB status to idle, set the event flag, and return the actual count to the requestor's variable. 
	COM1_control_block->oper_status = IDLE;
	*(COM1_control_block->eflag_p) = 1; // again, not sure what it wants here but i think its this
	*count = actual_count;
	// TODO: should we be nice and null-terminate before returning? or will that break something

	return 0;
}

int com_write(char *buf, int *count) {

}