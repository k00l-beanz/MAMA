#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef enum {
	READ_REQUEST,

	WRITE_REQUEST
} iocb_type_t;

typedef struct iocb_t {
	iocb_type_t type;

	pcb_t *pcb;

	char *buf;

	int *count;

	struct iocb_t *next;	
} iocb_t;

void io_refresh_queue();

void io_enqueue_read(pcb_t *, char *, int *);

void io_enqueue_write(pcb_t *, char *, int *);

void io_enqueue(iocb_t *);

void io_dequeue();

void io_try_start_next();

#endif