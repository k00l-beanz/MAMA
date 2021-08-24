#define MAX_HOURS 23
#define MAX_MINUTES 59
#define MAX_SECONDS 59

#define MAX_YEAR 99
#define MAX_MONTH 12
#define MAX_DAY 31
#define MAX_DAY_OF_WEEK 6

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
unsigned char ItoBCD(unsigned int value);

/*
 * Procedure: BCDtoI()
 * Description: Converts 8-bit BCD into 
*/
unsigned int BCDtoI(unsigned int num);

/*
 * Procedure: setdate()
 * Description: Set the system date
*/
int setdate(unsigned int year, unsigned int month, unsigned int day, unsigned int dayOfWeek);

/*
 * Procedure: getdate()
 * Description: Get the system date
*/
void getdate();

/*
 * Procedure: getMonth()
 * Description: Convert int representation of month
 *  to a name.
 *  0 = January
 *  11 = December 
*/
char * getMonth(int value);

/*
 * Procedure: getDayOfWeek()
 * Description: Convert int representation of day of the week
 *  to a name.
 *  0 = January
 *  11 = December
*/
char * getDayOfWeek(int value);