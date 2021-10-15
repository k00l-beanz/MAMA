#include "../include/system.h"
#include "../include/core/serial.h"

#include "../modules/mpx_supt.h"
#include "procsr3.h"

#define RC_1 1
#define RC_2 2
#define RC_3 3
#define RC_4 4
#define RC_5 5

char* msg1 = "proc1 dispatched";
char* msg2 = "proc2 dispatched";
char* msg3 = "proc3 dispatched";
char* msg4 = "proc4 dispatched";
char* msg5 = "proc5 dispatched";
int msgSize = 17;

char* er1 = "proc1 ran after it was terminated";
char* er2 = "proc2 ran after it was terminated";
char* er3 = "proc3 ran after it was terminated";
char* er4 = "proc4 ran after it was terminated";
char* er5 = "proc5 ran after it was terminated";
int erSize = 34;

void proc1()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_1; i++){
      //serial_println("proc1 dispatched");
	sys_req(WRITE, DEFAULT_DEVICE, msg1, &msgSize);
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
      sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    //serial_println("proc1 ran after it was terminated");
	sys_req(WRITE, DEFAULT_DEVICE, er1, &erSize);
  }
}

void proc2()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_2; i++){
      sys_req(WRITE, DEFAULT_DEVICE, msg2, &msgSize);
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
      sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    sys_req(WRITE, DEFAULT_DEVICE, er2, &erSize);
  }
}

void proc3()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_3; i++){
      sys_req(WRITE, DEFAULT_DEVICE, msg3, &msgSize);
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
      sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    sys_req(WRITE, DEFAULT_DEVICE, er3, &erSize);
  }
}

void proc4()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_4; i++){
      sys_req(WRITE, DEFAULT_DEVICE, msg4, &msgSize);
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
      sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    sys_req(WRITE, DEFAULT_DEVICE, er4, &erSize);
  }
}

void proc5()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_5; i++){
      sys_req(WRITE, DEFAULT_DEVICE, msg5, &msgSize);
      sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
      sys_req(EXIT, DEFAULT_DEVICE, NULL, NULL);
    sys_req(WRITE, DEFAULT_DEVICE, er5, &erSize);
  }
}
