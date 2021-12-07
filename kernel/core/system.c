#include <string.h>

#include <system.h>

#include <core/serial.h>

#include <modules/mpx_supt.h>

#include "term/pcb/pcb.h"

#include "term/dispatch/context.h"

#include <lib/out.h>

#include <io_scheduler/scheduler.c>

/// Currently operating process
pcb_t * cop;

/// Context 
context * global_context;
extern pcb_queue_t * priority_queue;
extern param params;

/*
  Procedure..: klogv
  Description..: Kernel log messages. Sent to active
      serial device.
*/
void klogv(const char * msg) {
    char logmsg[64] = {
        '\0'
    }, prefix[] = "klogv: ";
    strcat(logmsg, prefix);
    strcat(logmsg, msg);
    serial_println(logmsg);
}

/*
  Procedure..: kpanic
  Description..: Kernel panic. Prints an error message
      and halts.
*/
void kpanic(const char * msg) {
    cli(); //disable interrupts
    char logmsg[64] = {
        '\0'
    }, prefix[] = "Panic: ";
    strcat(logmsg, prefix);
    strcat(logmsg, msg);
    klogv(logmsg);
    hlt(); //halt
}

/**
 * Called to start interrupt
 * 
 * Is called by irq to determine the next routine
 * to load
 * 
 * @param registers Context registers for the current process
 * @return Pointer to the process being loaded
 * 
 */
u32int * sys_call(context * registers) {
    serial_println("top of sys call");
    io_refresh_queue();

    pcb_t * pcb = NULL;

	// fetch next node to switch to, remove from ready queue
	pcb_node_t * node = priority_queue -> pcbq_head;
	while (node != NULL && node -> pcb -> pcb_process_state != READY) {
		node = node -> pcbn_next_pcb;
	}
	if(node != NULL) {
		pcb = node -> pcb;
		removePCB(pcb);
	}

    //Is there a currently operating process? 
    if (cop == NULL) {
        global_context = registers;
    } else {
		//There is an existing cop 

        if (params.op_code == IDLE) {
            // Save the context of cop
            cop -> pcb_stack_top = (unsigned char * ) registers;
            cop -> pcb_process_state = READY;
            insertPCB(cop);
        } else if (params.op_code == EXIT) {
            // Free cop
            cop -> pcb_process_state = SUSPENDED_READY;
            removePCB(cop);
            freePCB(cop);

            cop = NULL;
        } else if(params.op_code == READ) {
            io_enqueue_read(cop, params.buffer_ptr, params.count_ptr);
            // save cop context, block until I/O request complete
            cop -> pcb_stack_top = (unsigned char * ) registers;
            cop -> pcb_process_state = BLOCKED;
            insertPCB(cop);
    	} else if(params.op_code == WRITE) {
            io_enqueue_write(cop, params.buffer_ptr, params.count_ptr);
            // save cop context, block until I/O request complete
            cop -> pcb_stack_top = (unsigned char * ) registers;
            cop -> pcb_process_state = BLOCKED;
            insertPCB(cop);
    	}
    }

    io_try_start_next();

    serial_println("left io_try_start_next");

    // There is a READY pcb
    if (pcb != NULL) {
        cop = pcb;
        cop -> pcb_process_state = RUNNING;
        serial_println("next PCB to schedule: ");
        serial_println(pcb->pcb_name);
        serial_println("about to return from sys_call");
        return (u32int * ) cop -> pcb_stack_top;
    }
    return (u32int * ) global_context;
}
