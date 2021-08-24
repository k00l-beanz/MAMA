
/*
 * Procedure: settime()
 * Description: Set the time of the system.
*/
int settime(int hour, int minute, int second);

/*
 * Procedure: gettime()
 * Description: Get the time of the system.
*/
void gettime();

/*
 * Procedure: ItoBCD()
 * Description: Converts 32-bit integer into 8-bit BCD
*/
static unsigned char ItoBCD(unsigned int value);

/*
 * Procedure: BCDtoI()
 * Description: Converts 8-bit BCD into 
*/
static unsigned int BCDtoI(unsigned int num);