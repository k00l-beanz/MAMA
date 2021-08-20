/*
  ----- interrupt.c -----

  Description..: Interrupt handling routines for
    traps, gates, exceptions.
*/

#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>

// Programmable Interrupt Controllers
#define PIC1 0x20
#define PIC2 0xA0

// Initialization Code Words
#define ICW1 0x11
#define ICW4 0x01

/*
  Procedure..: io_wait
  Description..: The i386 can do an io wait by accessing another port.
      Mainly used in initializing the PIC.
*/
#define io_wait() asm volatile ("outb $0x80")

extern void divide_error();
extern void debug();
extern void nmi();
extern void breakpoint();
extern void overflow();
extern void bounds();
extern void invalid_op();
extern void device_not_available();
extern void double_fault();
extern void coprocessor_segment();
extern void invalid_tss();
extern void segment_not_present();
extern void stack_segment();
extern void general_protection();
extern void page_fault();
extern void reserved();
extern void coprocessor();
extern void rtc_isr();

extern idt_entry idt_entries[256];

//Current serial handler
extern void isr0();
void do_isr()
{
  char in = inb(COM2);
  serial_print(&in);
  serial_println("here");
  outb(0x20,0x20); //EOI
}

/*
  Procedure..: irq_init
  Description..: Installs the initial interrupt handlers for
      the first 32 irq lines. Most do a panic for now.
*/
void init_irq(void)
{  
  int i;

  // Necessary interrupt handlers for protected mode
  u32int isrs[17] = {
    (u32int)divide_error,
    (u32int)debug,
    (u32int)nmi,
    (u32int)breakpoint,
    (u32int)overflow,
    (u32int)bounds,
    (u32int)invalid_op,
    (u32int)device_not_available,
    (u32int)double_fault,
    (u32int)coprocessor_segment,
    (u32int)invalid_tss,
    (u32int)segment_not_present,
    (u32int)stack_segment,
    (u32int)general_protection,
    (u32int)page_fault,
    (u32int)reserved,
    (u32int)coprocessor
  };

  // Install handlers; 0x08=sel, 0x8e=flags
  for(i=0; i<32; i++){
    if (i<17) idt_set_gate(i, isrs[i], 0x08, 0x8e);
    else idt_set_gate(i, (u32int)reserved, 0x08, 0x8e);
  }
  // Ignore interrupts from the real time clock
  idt_set_gate(0x08, (u32int)rtc_isr, 0x08, 0x8e);
}

/*
  Procedure..: pic_init
  Description..: Initializes the programmable interrupt controllers
      and performs the necessary remapping of IRQs. Leaves interrupts
      turned off.
*/
void init_pic(void)
{
  outb(PIC1,ICW1);   //send initialization code words 1 to PIC1
  io_wait();
  outb(PIC2,ICW1);   //send icw1 to PIC2
  io_wait();
  outb(PIC1+1,0x20); //icw2: remap irq0 to 32
  io_wait();
  outb(PIC2+1,0x28); //icw2: remap irq8 to 40
  io_wait();
  outb(PIC1+1,4);    //icw3
  io_wait();
  outb(PIC2+1,2);    //icw3
  io_wait();
  outb(PIC1+1,ICW4); //icw4: 80x86, automatic handling
  io_wait();
  outb(PIC2+1,ICW4); //icw4: 80x86, automatic handling
  io_wait();
  outb(PIC1+1,0xFF); //disable irqs for PIC1
  io_wait();
  outb(PIC2+1,0xFF); //disable irqs for PIC2
}

void do_divide_error()
{
  kpanic("Division-by-zero");
}
void do_debug()
{
  kpanic("Debug");
}
void do_nmi()
{
  kpanic("NMI");
}
void do_breakpoint()
{
  kpanic("Breakpoint");
}
void do_overflow()
{
  kpanic("Overflow error");
}
void do_bounds()
{
  kpanic("Bounds error");
}
void do_invalid_op()
{
  kpanic("Invalid operation");
}
void do_device_not_available()
{
  kpanic("Device not available");
}
void do_double_fault()
{
  kpanic("Double fault");
}
void do_coprocessor_segment()
{
  kpanic("Coprocessor segment error");
}
void do_invalid_tss()
{
  kpanic("Invalid TSS");
}
void do_segment_not_present()
{
  kpanic("Segment not present");
}
void do_stack_segment()
{
  kpanic("Stack segment error");
}
void do_general_protection()
{
  kpanic("General protection fault");
}
void do_page_fault()
{
  kpanic("Page Fault");
}
void do_reserved()
{
  serial_println("die: reserved");
}
void do_coprocessor()
{
  kpanic("Coprocessor error");
}
