#ifndef _SYSTEM_H
#define _SYSTEM_H

#define NULL 0

// Suppress 'unused parameter' warnings/errors
#define no_warn(p) if (p) while (1) break

// Allows compilation with gcc -std=c89
// May also help avoid naming conflicts
#define asm __asm__
#define volatile __volatile__

#define sti()  asm volatile ("sti"::)  //turn irqs off
#define cli()  asm volatile ("cli"::)  //turn irqs on
#define nop()  asm volatile ("nop"::)  //skip cycle
#define hlt()  asm volatile ("hlt"::)  //halt
#define iret() asm volatile ("iret"::) //interrupt return

#define GDT_CS_ID 0x01 //kernel code segment ID
#define GDT_DS_ID 0x02 //kernel data segment ID

/* System Types */
typedef unsigned int   size_t;
typedef unsigned char  u8int;
typedef unsigned short u16int;
typedef unsigned long  u32int;

/* Time */
typedef struct {
  int sec;
  int min;
  int hour;
  int day_w;
  int day_m;
  int day_y;
  int mon;
  int year;
} date_time;

/* Test if interrupts are on */
static inline int irq_on()
{
  int f;
  asm volatile ("pushf\n\t"
		"popl %0"
		: "=g"(f));
  return f & (1 << 9);
}

void klogv(const char *msg);
void kpanic(const char *msg);

#endif
