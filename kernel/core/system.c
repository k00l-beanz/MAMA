#include <string.h>
#include <system.h>

#include <core/serial.h>
#include <modules/mpx_supt.c>

#include "term/pcb/pcb.h"
#include "term/pcb/pcb.c"
#include "term/dispatch/context.h"

/// Currently operating process
pcb_t * cop;

/// Context 
context * global_context;

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
      } else if (params.op_code == EXIT) {
        // Free cop
        freePCB(cop);
      }
  }

  // Get head of READY queue 
  pcb_t * pcb = priority_queue->pcbq_head->pcb;
  
  if (pcb != NULL) {
    cop = pcb;
    removePCB(pcb);
    return (u32int *) cop->pcb_stack_top;
  }

  return (u32int *) global_context;
}