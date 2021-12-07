#include <io_scheduler/scheduler.h>
#include <serial_driver/driver.c>

#define DEFAULT_BAUD_RATE 1200

iocb_t *COM1_queue;

void io_refresh_queue() {
	serial_println("top of io_refresh_queue");
	iocb_t *node = COM1_queue;
	while(node != NULL) {
		if(node->e_flag) {
			removePCB(node->pcb);
			node->pcb->pcb_process_state = READY;
			insertPCB(node->pcb);
		}
		iocb_t *prev = node;
		node = node->next;
		io_dequeue(prev);
	}
	if(COM1_control_block != NULL && COM1_control_block->oper_status == DEVICE_IDLE) {
		com_close();
	}
}

void io_enqueue_read(pcb_t *pcb, char *buf, int *count) {
	iocb_t *new_block = sys_alloc_mem(sizeof(iocb_t));
	new_block->type = READ_REQUEST;
	new_block->pcb = pcb;
	new_block->e_flag = 0;
	new_block->buf = buf;
	new_block->count = count;
	new_block->next = NULL;
	io_enqueue(new_block);
}

void io_enqueue_write(pcb_t *pcb, char *buf, int *count) {
	iocb_t *new_block = sys_alloc_mem(sizeof(iocb_t));
	new_block->type = WRITE_REQUEST;
	new_block->pcb = pcb;
	new_block->e_flag = 0;
	new_block->buf = buf;
	new_block->count = count;
	new_block->next = NULL;
	io_enqueue(new_block);
}

void io_enqueue(iocb_t *control_block) {
	if(COM1_queue == NULL) {
		COM1_queue = control_block;
	} else {
		iocb_t *prev = COM1_queue;
		while(prev->next != NULL)
			prev = prev->next;
		prev->next = control_block;
	}
}

void io_dequeue(iocb_t *control_block) {
	if(control_block == COM1_queue) {
		COM1_queue = control_block->next;
		sys_free_mem(control_block);
	}
	else {
		iocb_t *prev = COM1_queue;
		while(prev != NULL && prev->next != control_block)
			prev = prev->next;

		iocb_t *prev_next = prev->next;
		if(prev_next == control_block) {
			prev->next = control_block->next;
			sys_free_mem(control_block);
		} else {
			// error
			return;
		}
	}
}

void io_try_start_next() {
	if(COM1_control_block == NULL || COM1_control_block->ready_state == CLOSED) {
		serial_println("try start next: ready for next");
		iocb_t *next = COM1_queue;
		if(next != NULL) {
			serial_println("try start next: COM1 queue not empty");
			com_open(&(next->e_flag), DEFAULT_BAUD_RATE);
			serial_println("cp 1");
			if(next->type == READ_REQUEST) {
				serial_println("cp 2");
				com_read(next->buf, next->count);
			} else {
				serial_println("cp 3");
				com_write(next->buf, next->count);
			}
		} else { serial_println("try start next: COM1 queue empty "); }
	} else { serial_println("try start next: not ready for next"); }
}