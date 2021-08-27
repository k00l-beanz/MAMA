
#include "dnt.h"

int daysInMonth(int, int);

int setdate(char * date) {
  int params = 1;
  int month, day, year;
  char *sep = ".";

  char *token = strtok(date,sep);
  month = atoi(token);

  /*
  if (sizeof(token) != sizeof(char *)) {
    print("Error: Incorrect Format. See help pages for more information\n",61);
    return -1;
  }
  */

  while (token != NULL) {
    token = strtok(NULL,sep);

    switch (params) {
      case 1:
        day = atoi(token);
      case 2:
        year = atoi(token);
      default:
        break;
    }

    params++;
  }

  if (params != 4) {
    print("Error: Incorrect format. See help page for more information\n",61);
    return -1;
  }

  setDateInMemory(month, day, year);

  return 1;
}

int setDateInMemory(int month,int day,int year) {
  /* Verify that the user entered a date that is w/i bounds */
  if (year  > MAX_YEAR  || year  < MIN_YEAR  ||
      month > MAX_MONTH || month < MIN_MONTH ||
      day   > daysInMonth(month, year) || day < MIN_DAY) {
    print("Error: Date is not within bounds. See help page for more information.\n",63);
    return 0;
  }

  int day_of_week = EPOCH_FIRST_DAY_OF_WEEK_OF_YEAR;
  int i;
  for(i = EPOCH_YEAR; i < year; i++) {
    if(i % 4 == 0)
      day_of_week += DAYS_IN_LEAP_YEAR;
    else
      day_of_week += DAYS_IN_YEAR;
  }
  for(i = EPOCH_FIRST_MONTH_OF_YEAR; i < month; i++) {
    day_of_week += daysInMonth(month, year);
  }
  for(i = EPOCH_FIRST_DAY_OF_YEAR; i < day; i++) {
    day_of_week++;
  }
  day_of_week = (day_of_week - 1) % 7 + 1;

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
  BCDdayOfWeek = ItoBCD(day_of_week);
  outb(0x71,BCDdayOfWeek);

  return 1;
}

int getdate(char * p) {
  (void) p;

  char output[100];
  char *comma = ", ";
  char *twenty = "20";
  char *NL = "\n", *space = " ";
  int year, month, day, dayOfWeek;

  memset(output,'\0',100);

  /* Get day of week */
  outb(0x70,0x06);
  dayOfWeek = BCDtoI(inb(0x71));
  strcat(output,intToDayOfWeek(dayOfWeek));
  strcat(output,comma);
  
  /* Get month */
  outb(0x70, 0x08);
  month = BCDtoI(inb(0x71));
  strcat(output,intToMonth(month));
  strcat(output,space);

  /* Get day */
  outb(0x70,0x07);
  day = BCDtoI(inb(0x71));
  strcat(output,itoa(day));
  strcat(output,comma);
  strcat(output,twenty);

  /* Get year */
  outb(0x70,0x09);
  year = BCDtoI(inb(0x71));
  strcat(output,itoa(year));

  strcat(output,NL);

  sys_req(WRITE,DEFAULT_DEVICE,output,&year);

  return 0;
}

int settime(char * time) {
  int params = 1, hour, minute, second;
  char *token, *sep = ".";

  token = strtok(time,sep);
  hour = atoi(token);

  while (token != NULL) {
    token = strtok(NULL,sep);

    switch (params) {
      case 1:
        minute = atoi(token);
      case 2:
        second = atoi(token);
      default:
        break;
    }

    params++;
  }

  if (params != 4) {
    print("Error: Incorrect format. See help page for more information.\n",61);
    return -1;
  }

  setTimeInMemory(hour, minute, second);

  return 0;
}

void setTimeInMemory(int hour, int minute, int second) {
  unsigned char BCDhour, BCDminute, BCDsecond;

  cli();

  // Verify that the user entered time that is w/i bounds
  if ((hour > MAX_HOURS || hour < MIN) || (minute > MAX_MINUTES || minute < MIN) || (second > MAX_SECONDS || second < MIN)) {
    print("Error: Time is not within bounds. See help page for more information.\n",70);
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

int gettime(char * p) {
  (void) p;

  int hour, minute, second;
  char *semi = ":";
  char *NL = "\n";
  char output[20];

  memset(output,'\0',20);

  // Hours
  outb(0x70, 0x04);
  hour = BCDtoI(inb(0x71));
  strcat(output,itoa(hour));
  strcat(output,semi);

  // Minutes
  outb(0x70,0x02);
  minute = BCDtoI(inb(0x71));
  strcat(output,itoa(minute));
  strcat(output,semi);

  // Seconds
  outb(0x70,0x00);
  second = BCDtoI(inb(0x71));
  strcat(output,itoa(second));

  strcat(output,NL);
  sys_req(WRITE,DEFAULT_DEVICE,output,&hour);

  return 0;
}

unsigned char ItoBCD(unsigned int value) {
  return (unsigned char) ((value / 10) << 4) | (value % 10);
}

unsigned int BCDtoI(unsigned char value) {
  return (unsigned int) (((value >> 4) * 10) + (value & 0x0F));
}

char * intToMonth(int value) {
  char * month;
  switch(value) {
    case 1:
      month = "January";
      break;
    case 2:
      month = "February";
      break;
    case 3:
      month = "March";
      break;
    case 4:
      month = "April";
      break;
    case 5:
      month = "May";
      break;
    case 6:
      month = "June";
      break;
    case 7:
      month = "July";
      break;
    case 8:
      month = "August";
      break;
    case 9:
      month = "September";
      break;
    case 10:
      month = "October";
      break;
    case 11:
      month = "November";
      break;
    case 12:
      month = "December";
      break;
    default:
      month = "";
  }
  return month;
}

char * intToDayOfWeek(int value) {
  char *dayOfWeek;
  switch(value) {
    case 1:
      dayOfWeek = "Sunday";
      break;
    case 2:
      dayOfWeek = "Monday";
      break;
    case 3:
      dayOfWeek = "Tuesday";
      break;
    case 4:
      dayOfWeek = "Wednesday";
      break;
    case 5:
      dayOfWeek = "Thursday";
      break;
    case 6:
      dayOfWeek = "Friday";
      break;
    case 7:
      dayOfWeek = "Saturday";
      break;
    default:
      dayOfWeek = "";
  }

  return dayOfWeek;
}

int daysInMonth(int month, int year) {
switch(month) {
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
        return 31;
      case 4:
      case 6:
      case 9:
      case 11:
        return 30;
      case 2:
        if(year % 4 == 0)
          return 29;
        else
          return 28;
        break;
    }
return -1; // will never happen
}

