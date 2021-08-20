#ifndef _IO_H
#define _IO_H

/*
  Procedure..: outb
  Description..: Write a byte of data to a port.
*/
#define outb(port, data)					\
  asm volatile ("outb %%al,%%dx" : : "a" (data), "d" (port))

/*
  Procedure..: inb
  Description..: Read a byte of data from a port.
*/
#define inb(port) ({						\
      unsigned char r;						\
      asm volatile ("inb %%dx,%%al": "=a" (r): "d" (port));	\
      r;							\
    })

#endif
