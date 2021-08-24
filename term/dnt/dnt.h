#define MAX_HOURS 23
#define MAX_MINUTES 59
#define MAX_SECONDS 59

#define MAX_YEAR 99
#define MAX_MONTH 12
#define MAX_DAY 31
#define MAX_DAY_OF_WEEK 6

/*
 * Procedure: setdate()
 * Description: Set the system date.
 *  Syntax: DayOfWeek.Month.Day.Year
 *  Ex:     Tuesday.August.24.21
 *  #TODO: Error Handling
*/
int setdate(char * date);

/*
 * Procedure: setDateinMemory()
 * Description: Set the date in memory.
*/
void setDateInMemory(unsigned int dayOfWeek, unsigned int month, unsigned int day, unsigned int year);

/*
 * Procedure: getdate()
 * Description: Get the system date
*/
int getdate(char * p);

/*
 * Procedure: settime()
 * Description: Set the system time
 * Syntax:  Hour.Minute.Second
 * Ex:      18.58.12
 * #TODO: Error Handling
*/
int settime(char * time);

/*
 * Procedure setTimeInMemory()
 * Descriptoin: Set the system time in memory
*/
void setTimeInMemory(unsigned int hour, unsigned int minute, unsigned int second);

/*
 * Procedure: gettime()
 * Description: Get the system time.
*/
int gettime(char * p);

/*
 * Procedure: ItoBCD()
 * Description: Converts 32-bit int into 8-bit BCD
*/
unsigned char ItoBCD(unsigned int value);

/*
 * Procedure: BCDtoI()
 * Description: Converts 8-bit BCD into 32-bit int
*/
unsigned int BCDtoI(unsigned char value);

/*
 * Procedure: intToMonth()
 * Description: Convert int representation of month
 *  to a name.
 *  0 = January
 *  11 = December 
*/
char * intToMonth(int value);

/*
 * Procedure: intToDayOfWeek()
 * Description: Convert int representation of day of the week
 *  to a name.
 *  0 = January
 *  11 = December
*/
char * intToDayOfWeek(int value);

/*
 * Procedure: dayOfWeekToInt()
 * Description: Converts user input dayofweek to int
*/
int dayOfWeekToInt(char * dayofweek);

/*
 * Procedure: monthToInt()
 * Description: Converts user supplied month to int
*/
int monthToInt(char * month);

/*
 * Procedure: getMonth()
 * Description: Convert int representation of month
 *  to a name.
 *  0 = January
 *  11 = December 
*/
char * intToMonth(int value);