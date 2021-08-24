
#include "dnt.h"

/*
 * Procedure: setdate()
 * Description: Set the system date.
 *  Syntax: DayOfWeek.Month.Day.Year
 *  Ex:     Tuesday.August.24.21
*/
int setdate(char * date) {
  int i;
  int dayOfWeek, month, day, year;
  char *sep = ".";

  char *token = strtok(date,sep);
  dayOfWeek = dayOfWeekToInt(token);

  /* Extract rest of date */
  for (i = 0; i < 3; i++) { 
    token = strtok(NULL,sep);

    if (i == 0) { // Month
      month = monthToInt(token);
    }
    else if (i == 1) { // Day
      day = atoi(token);
    }
    else if (i == 2) { // Year
      year = atoi(token);
    }
  }

  setDateInMemory(dayOfWeek, month, day, year);

  return 0;
}

/*
 * Procedure: setDateinMemory()
 * Description: Set the date in memory.
 * #TODO: Error Handling
*/
void setDateInMemory(unsigned int dayOfWeek, unsigned int month, unsigned int day, unsigned int year) {
  /* Verify that the user entered a date that is w/i bounds */
  if ((year > MAX_YEAR) || (month > MAX_MONTH) || (day > MAX_DAY) || (dayOfWeek > MAX_DAY_OF_WEEK)) {
    return;
  }

  unsigned char BCDyear, BCDmonth, BCDday, BCDdayOfWeek;

  /* Set year */
  outb(0x70,0x09);
  BCDyear = ItoBCD(year);
  outb(0x71,BCDyear);

  /* Set month */
  outb(0x70,0x08);
  BCDmonth = ItoBCD(month);
  outb(0x71,BCDmonth);

  /* Set day */
  outb(0x70,0x07);
  BCDday = ItoBCD(day);
  outb(0x71,BCDday);

  /* Set day of week */
  outb(0x70,0x06);
  BCDdayOfWeek = ItoBCD(dayOfWeek);
  outb(0x71,BCDdayOfWeek);
}

int getdate(char * p) {
  (void) p;

  char output[100];
  char *comma = ", ";
  char *twenty = "20";
  char *NL = "\n";
  int year, month, day, dayOfWeek;

  memset(output,'\0',100);

  /* Get day of week */
  outb(0x70,0x06);
  dayOfWeek = BCDtoI((unsigned char) inb(0x71));
  strcat(output,intToDayOfWeek(dayOfWeek));
  strcat(output,comma);
  
  /* Get month */
  outb(0x70, 0x08);
  month = BCDtoI((unsigned char) inb(0x71));
  strcat(output,intToMonth(month));

  /* Get day */
  outb(0x70,0x07);
  day = BCDtoI((unsigned char) inb(0x71));
  strcat(output,itoa(day));
  strcat(output,comma);
  strcat(output,twenty);

  /* Get year */
  outb(0x70,0x09);
  year = BCDtoI((unsigned char) inb(0x71));
  strcat(output,itoa(year));

  strcat(output,NL);

  sys_req(WRITE,DEFAULT_DEVICE,output,&year);

  return 0;
}

/*
 * Procedure: settime()
 * Description: Set the system time
 * Syntax:  Hour.Minute.Second
 * Ex:      18.58.12
*/
int settime(char * time) {
  int i, hour, minute, second;
  char *token, *sep = ".";

  token = strtok(time,sep);
  hour = atoi(token);

  for (i = 0; i < 2; i++) {
    token = strtok(NULL,sep);

    if (i == 0) {
      minute = atoi(token);
    }
    else if (i == 1) {
      second = atoi(token);
    }
  }

  setTimeInMemory(hour, minute, second);

  return 0;
}

/*
 * Procedure setTimeInMemory()
 * Description: Set the system time in memory
 * #TODO: Error Handling
*/
void setTimeInMemory(unsigned int hour, unsigned int minute, unsigned int second) {
  unsigned char BCDhour, BCDminute, BCDsecond;

  cli();

  // Verify that the user entered time that is w/i bounds
  if ((hour > MAX_HOURS) || (minute > MAX_MINUTES) || (second > MAX_SECONDS)) {
    return;
  }

  // Set the hours
  outb(0x70,0x04);
  BCDhour = ItoBCD(hour);
  outb(0x71,BCDhour);

  // Set minutes
  outb(0x70,0x02);
  BCDminute = ItoBCD(minute);
  outb(0x71,BCDminute);
  
  // Set Seconds
  outb(0x70,0x00);
  BCDsecond = ItoBCD(second);
  outb(0x71,BCDsecond);

  sti();

}

/*
 * Procedure: gettime()
 * Description: Get the system time.
*/
int gettime(char * p) {
  (void) p;

  int hour, minute, second;
  char *semi = ":";
  char *NL = "\n";
  char output[20];

  memset(output,'\0',20);

  // Hours
  outb(0x70, 0x04);
  hour = BCDtoI((unsigned char) inb(0x71));
  strcat(output,itoa(hour));
  strcat(output,semi);

  // Minutes
  outb(0x70,0x02);
  minute = BCDtoI((unsigned char) inb(0x71));
  strcat(output,itoa(minute));
  strcat(output,semi);

  // Seconds
  outb(0x70,0x00);
  second = BCDtoI((unsigned char) inb(0x71));
  strcat(output,itoa(second));
  strcat(output,semi);

  strcat(output,NL);
  sys_req(WRITE,DEFAULT_DEVICE,output,&hour);

  return 0;
}

/*
 * Procedure: ItoBCD()
 * Description: Converts 32-bit int into 8-bit BCD
*/
unsigned char ItoBCD(unsigned int value) {
  return (unsigned char) ((value / 10) << 4) | (value % 10);
}

/*
 * Procedure: BCDtoI()
 * Description: Converts 8-bit BCD into 32-bit int
*/
unsigned int BCDtoI(unsigned char value) {
  return (unsigned int) (((value >> 4) * 10) + (value & 0x0F));
}

/*
 * Procedure: getMonth()
 * Description: Convert int representation of month
 *  to a name.
 *  0 = January
 *  11 = December 
*/
char * intToMonth(int value) {
  char * month;
  switch(value) {
    case 0:
      month = "January";
      break;
    case 1:
      month = "February";
      break;
    case 2:
      month = "March";
      break;
    case 3:
      month = "April";
      break;
    case 4:
      month = "May";
      break;
    case 5:
      month = "June";
      break;
    case 6:
      month = "July";
      break;
    case 7:
      month = "August";
      break;
    case 8:
      month = "September";
      break;
    case 9:
      month = "October";
      break;
    case 10:
      month = "November";
      break;
    case 11:
      month = "December";
    default:
      month = "";
  }
  return month;
}

/*
 * Procedure: intToDayOfWeek()
 * Description: Convert int representation of day of the week
 *  to a name.
 *  0 = January
 *  11 = December
*/
char * intToDayOfWeek(int value) {
  char *dayOfWeek;
  switch(value) {
    case 0:
      dayOfWeek = "Sunday";
      break;
    case 1:
      dayOfWeek = "Monday";
      break;
    case 2:
      dayOfWeek = "Tuesday";
      break;
    case 3:
      dayOfWeek = "Wednesday";
      break;
    case 4:
      dayOfWeek = "Thursday";
      break;
    case 5:
      dayOfWeek = "Friday";
      break;
    case 6:
      dayOfWeek = "Saturday";
      break;
    default:
      dayOfWeek = "";
  }

  return dayOfWeek;
}

/*
 * Procedure: dayOfWeekToInt()
 * Description: Converts user input dayofweek to int
*/
int dayOfWeekToInt(char * dayofweek) {
  if (strcmp(dayofweek,"Sunday") == 0) {
    return 0;
  }
  else if (strcmp(dayofweek,"Monday") == 0) {
    return 1;
  }
  else if (strcmp(dayofweek,"Tuesday") == 0) {
    return 2;
  }
  else if (strcmp(dayofweek,"Wednesday") == 0) {
    return 3;
  }
  else if (strcmp(dayofweek,"Thursday") == 0) {
    return 4;
  }
  else if (strcmp(dayofweek,"Friday") == 0) {
    return 5;
  }
  else if (strcmp(dayofweek,"Saturday") == 0) {
    return 6;
  }

  return -1;
}

/*
 * Procedure: monthToInt()
 * Description: Converts user supplied month to int
*/
int monthToInt(char * dayofweek) {
  if (strcmp(dayofweek,"January") == 0) {
    return 0;
  }
  else if (strcmp(dayofweek,"February") == 0) {
    return 1;
  }
  else if (strcmp(dayofweek,"March") == 0) {
    return 2;
  }
  else if (strcmp(dayofweek,"April") == 0) {
    return 3;
  }
  else if (strcmp(dayofweek,"May") == 0) {
    return 4;
  }
  else if (strcmp(dayofweek,"June") == 0) {
    return 5;
  }
  else if (strcmp(dayofweek,"July") == 0) {
    return 6;
  }
  else if (strcmp(dayofweek,"August") == 0) {
    return 7;
  }
  else if (strcmp(dayofweek,"September") == 0) {
    return 8;
  }
  else if (strcmp(dayofweek,"October") == 0) {
    return 9;
  }
  else if (strcmp(dayofweek,"November") == 0) {
    return 10;
  }
  else if (strcmp(dayofweek,"December") == 0) {
    return 11;
  }

  return -1;
}