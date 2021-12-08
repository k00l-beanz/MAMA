
// Include your mpx_supt.h, serial, system, string
// any other necessary includes.

#include "context.h"

/*
  Procedure..: COMWRITE
  Description..: This process attempts to write a message to the 
  serial device, used for R6 Testing.  This should 
  be the first test process executed when testing 
  R6
  Params..: None
*/
void COMWRITE() {
  char msg[50];
  int count=0;

  memset(msg, '\0', sizeof(msg));
  strcpy(msg, "COMWRITE is writing a message.\n");
  count = strlen(msg);
 
  // write a message
  sys_req(WRITE, DEFAULT_DEVICE, msg, &count);
  // exit
  sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
 
}// end COMWRITE


/*
  Procedure..: COMREAD
  Description..: This process writes a prompt to the 
  serial device, and then reads user input
  which is then printed back to the device
  Params..: None
*/
void COMREAD() {
  char outputMsg[50];
  int outputCount=0;
  char inputBuffer[100];
  int inputCount=100;
	
  memset(outputMsg, '\0', sizeof(outputMsg));
  memset(inputBuffer, '\0', sizeof(inputBuffer));

  strcpy(outputMsg, "COMREAD: Please input a message.\n");
  outputCount = strlen(outputMsg);
 
  // write the prompt
  sys_req(WRITE, DEFAULT_DEVICE, outputMsg, &outputCount);
  // we should not need to IDLE
  //sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);

  // read the user input
  sys_req(READ, DEFAULT_DEVICE, inputBuffer, &inputCount);

  // echo the user input to the screen
  inputCount = strlen( inputBuffer);
  sys_req(WRITE, DEFAULT_DEVICE, inputBuffer, &inputCount);

  // exit
  sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);

}// end COMREAD

/*
  Procedure..: IOCOM25
  Description..: This process attempts to write a message to the 
  serial device 25 times and then exits
  Params..: None
*/
void IOCOM25() {
  char msg[50];
  int count=0;
  int printCount=0;
	
  memset(msg, '\0', sizeof(msg));
  strcpy(msg, "IOCOM25 Writing a message.\n");
  
  while (printCount < 25) {
    count = strlen(msg);
    // write a message
    sys_req(WRITE, DEFAULT_DEVICE, msg, &count);
    printCount++;
    sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
  }
  // exit
  sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
}// end IOCOM25

/*
  Procedure..: IOCOM
  Description..: This process attempts to write a message to the 
  serial device until suspended and terminated
  Params..: None
*/
void IOCOM() {
  char msg[50];
  int count=0;
	
  memset(msg, '\0', sizeof(msg));
  strcpy(msg, "IOCOM Writing a message.\n");
  count = strlen(msg);
  sys_req(WRITE, DEFAULT_DEVICE, msg, &count);

  memset(msg, '\0', sizeof(msg));
  strcpy(msg, "IOCOM Still Writing.\n");
  count = strlen(msg);
  
  while (1) {
    // Idles
    sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);

    // write a message
    sys_req(WRITE, DEFAULT_DEVICE, msg, &count);
    count = strlen(msg);
  }
  // exit
  sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
 
}// end IOCOM

int cmd_test_r6(char *args) {
	(void)args;

	pcb_t *pcb = dispatcher("COMWRITE", &COMWRITE);
  pcb->pcb_priority = 4;
  pcb->pcb_process_class = 0;
  pcb->pcb_process_state = READY;
  insertPCB(pcb);
	pcb = dispatcher("IOCOM25", &IOCOM25);
  pcb->pcb_priority = 4;
  pcb->pcb_process_class = 0;
  pcb->pcb_process_state = READY;
  insertPCB(pcb);
  pcb = dispatcher("COMREAD", &COMREAD);
  pcb->pcb_priority = 4;
  pcb->pcb_process_class = 0;
  pcb->pcb_process_state = READY;
  insertPCB(pcb);
	pcb = dispatcher("IOCOM", &IOCOM);
  pcb->pcb_priority = 4;
  pcb->pcb_process_class = 0;
  pcb->pcb_process_state = READY;
  insertPCB(pcb);

	return 0;
}
