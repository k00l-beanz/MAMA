#include <io_scheduler/scheduler.h>
#include <serial_driver/driver.c>
#include <kernel/core/kmain.h>

iocb_t *COM1_queue;

void io_refresh_queue() {
	//serial_println("top of io_refresh_queue");
	if(*(com1_eflag_ptr()) && COM1_queue != NULL) {
		*(com1_eflag_ptr()) = 0;
		if(COM1_queue == NULL) {
			serial_println("uhoh");
		}
		iocb_t *node = COM1_queue;
		//serial_println("found eflag");
		removePCB(node->pcb);
		node->pcb->pcb_process_state = READY;
		insertPCB(node->pcb);
		io_dequeue();
	}
}

void io_enqueue_read(pcb_t *pcb, char *buf, int *count) {
	//serial_println("enqueuing new read request");
	iocb_t *new_block = sys_alloc_mem(sizeof(iocb_t));
	new_block->type = READ_REQUEST;
	new_block->pcb = pcb;
	new_block->buf = buf;
	new_block->count = count;
	new_block->next = NULL;
	io_enqueue(new_block);
}

void io_enqueue_write(pcb_t *pcb, char *buf, int *count) {
	//serial_println("enqueuing new write request");
	iocb_t *new_block = sys_alloc_mem(sizeof(iocb_t));
	new_block->type = WRITE_REQUEST;
	new_block->pcb = pcb;
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

void io_dequeue() {
	//serial_println("dequeuing");
	if(COM1_queue != NULL) {
		iocb_t *old = COM1_queue;
		COM1_queue = COM1_queue->next;
		sys_free_mem(old);
	}
}

void io_try_start_next() {
	if(COM1_control_block == NULL) {
		//serial_println("dcb null");
	}
	if(COM1_control_block->oper_status == DEVICE_IDLE) {
		//serial_println("try start next: ready for next");
		iocb_t *next = COM1_queue;
		if(next != NULL) {
			//serial_println("try start next: COM1 queue not empty");
			//serial_println("cp 1");
			if(next->type == READ_REQUEST) {
				//serial_println("cp 2");
				com_read(next->buf, next->count);
			} else {
				//serial_println("cp 3");
				com_write(next->buf, next->count);
			}
		} else { /*serial_println("try start next: COM1 queue empty ");*/ }
	} else { /*serial_println("try start next: not ready for next");*/ }
}