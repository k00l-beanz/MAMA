#include <string.h>
#include <system.h>

#include <core/serial.h>

#include "term/pcb/pcb.h"
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

  /* Is there a currently operating process? */
  if (cop == NULL) {
    context = registers;
  }
  
  return registers; // Temporary
}