#include <string.h>
#include <system.h>

#include <core/serial.h>
#include <modules/mpx_supt.h>

#include "term/pcb/pcb.h"
#include "term/dispatch/context.h"

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
void klogv(const char *msg)
{
  char logmsg[64] = {'\0'}, prefix[] = "klogv: ";
  strcat(logmsg, prefix);
  strcat(logmsg, msg);
  serial_println(logmsg);
}

/*
  Procedure..: kpanic
  Description..: Kernel panic. Prints an error message
      and halts.
*/
void kpanic(const char *msg)
{
  cli(); //disable interrupts
  char logmsg[64] = {'\0'}, prefix[] = "Panic: ";
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
  
  //Is there a currently operating process? 
  if (cop == NULL) {
    global_context = registers;
  } else { //There is an existing cop 

      if (params.op_code == IDLE) {
        // Save the context
        cop->pcb_stack_top = (unsigned char *) registers;
        insertPCB(cop);
      } else if (params.op_code == EXIT) {
        // Free cop
        freePCB(cop);
        // TODO: cop now points to freed memory, 
        // using it will result in a use-after-free - does cop need to be set to null here? 
        // not really sure how any of this stuff works lol
        // 
        // I made it NULL to prevent what you mentioned above
        cop = NULL;
      }
  }

  // Get head of READY queue 
  pcb_node_t * node = priority_queue->pcbq_head;
  pcb_t * pcb;
  
  if (node == NULL) { // Priority queue is empty
    pcb = NULL;
  } else { // There is something in READY queue, find pcb that is READY

    while (node->pcb->pcb_process_state != READY) {
      node = node->pcbn_next_pcb;
    }
    pcb = node->pcb;
    pcb->pcb_process_state = RUNNING;
  }

  // There is a READY pcb
  if (pcb != NULL) {
    cop = pcb;
    removePCB(pcb);
    return (u32int *) cop->pcb_stack_top;
  }

  return (u32int *) global_context;
}