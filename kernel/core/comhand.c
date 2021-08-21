
#include <core/comhand.h>
#include <modules/mpx_supt.h>

int comhand() {
	char cmdBuffer[100];
	int bufferSize;
	int quit = 0;

	while (!quit) {
		
		bufferSize = 99;
		memset(cmdBuffer,'\0',100);
		cmdBuffer = "Hello World";
		sys_req(WRITE,DEFAULT_DEVICE,cmdBuffer,&bufferSize);
	}

	return 0;
}