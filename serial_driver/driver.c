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
#define INTERRUPT_ID_INPUT_READY_BIT 4

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

	char *user_write_buf;
	int *user_write_count;
	int user_write_progress;

	char ring_buffer[RING_BUFFER_SIZE];
	int ring_buffer_head;
	int ring_buffer_tail;
} dcb_t;

dcb_t *COM1_control_block = NULL;

int com_handle_interrupt();

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
	COM1_control_block->user_write_buf = NULL;
	COM1_control_block->user_write_count = NULL;
	COM1_control_block->user_write_progress = 0;
	COM1_control_block->ring_buffer_head = 0;
	COM1_control_block->ring_buffer_tail = 0;

	// 3.	Save the address of the current interrupt handler, and install the new handler in the interrupt vector.
	// TODO: save current address part
	idt_set_gate(0x24, (u32int)com_handle_interrupt, 0x08, 0x8e);

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
	cli();
	outb(PIC_MASK, inb(PIC_MASK) & ~0b10000); // PIC level for COM1 is 4, not sure if this is the right way to set it or not though
	sti();

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
	if(COM1_control_block->oper_status != DEVICE_IDLE) {
		return -304; // device busy
	}

	// 3.	Initialize the input buffer variables (not the ring buffer!) and set the status to reading. 
	int actual_count = 0;
	COM1_control_block->oper_status = DEVICE_READING;

	// 4.	Clear the caller's event flag. 
	*(COM1_control_block->eflag_p) = 0; // is this what it means?

	// 5.	Copy characters from the ring buffer to the requestor's buffer, until the ring buffer is emptied, the requested count has been reached, or a CR (ENTER) code has been found. The copied characters should, of course, be removed from the ring buffer. Either input interrupts or all interrupts should be disabled during the copying. 
	outb(BASE + INTERRUPT_ENABLE_REGISTER, 0x0); // disable input interrupts TODO: this might clear the write interrupt bit and not reset it

	while(actual_count < *count && COM1_control_block->ring_buffer_head != COM1_control_block->ring_buffer_tail) {
		char next = COM1_control_block->ring_buffer[COM1_control_block->ring_buffer_head];
		COM1_control_block->ring_buffer[COM1_control_block->ring_buffer_head] = '\0'; // I don't think this is actually necessary, but it says to do it
		COM1_control_block->ring_buffer_head = COM1_control_block->ring_buffer_head == RING_BUFFER_SIZE - 1 ? 0 : COM1_control_block->ring_buffer_head + 1;

		if(next == '\n')
			break;

		buf[actual_count] = next;
		actual_count++;
	}

	outb(BASE + INTERRUPT_ENABLE_REGISTER, 0x1); // reenable input interrupts

	// 6.	If more characters are needed, return. If the block is complete, continue with step 7. 
	if(actual_count < *count)
		return 0;

	// 7.	Reset the DCB status to idle, set the event flag, and return the actual count to the requestor's variable. 
	COM1_control_block->oper_status = DEVICE_IDLE;
	*(COM1_control_block->eflag_p) = 1; // again, not sure what it wants here but i think its this
	*count = actual_count;
	// TODO: should we be nice and null-terminate the caller's buffer before returning? or will that break something

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

int com_handle_interrupt_read() {
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
		//serial_println("done");
		COM1_control_block->oper_status = DEVICE_IDLE;
		*(COM1_control_block->eflag_p) = 1;
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
	} else {
		serial_println("some other interrupt");
	}

	outb(PIC_END_OF_INTERRUPT_REGISTER, PIC_SIGNAL_END_OF_INTERRUPT_CODE);
	//sti();
	//serial_println("bottom of com_handle_interrupt");
	return 0;
}