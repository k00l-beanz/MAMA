
#include <dnt.h>

/*
 * Procedure: ItoBCD()
 * Description: Converts 32-bit integer into 8-bit BCD
*/
static unsigned char ItoBCD(unsigned int value) {
	return ((value / 10) << 4) | (value % 10);
}

/*
 * Procedure: BCDtoI()
 * Description: Converts 8-bit BCD into 
*/
static unsigned int BCDtoI(unsigned int num) {
	return (((num >> 4) * 10) + (num & 0xF));
}