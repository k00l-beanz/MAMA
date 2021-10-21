#ifndef _TABLES_H
#define _TABLES_H

#include "system.h"

typedef struct idt_entry_struct
{
  u16int base_low;  //offset bits 0..15
  u16int sselect;   //stack selector in gdt or ldt
  u8int  zero;      //this stays zero; unused
  u8int  flags;     //attributes
  u16int base_high; //offset bits 16..31
}
  __attribute__ ((packed)) idt_entry;

typedef struct idt_struct
{
  u16int limit;
  u32int base;
}
  __attribute__ ((packed)) idt_descriptor;

typedef struct gdt_descriptor_struct
{
  u16int limit;
  u32int base;
}
  __attribute__ ((packed)) gdt_descriptor;

typedef struct gdt_entry_struct
{
  u16int limit_low; //first 16 bits of limit
  u16int base_low;  //first 16 bits of base
  u8int  base_mid;  //bits 16-23 of base
  u8int  access;    //next 8 bits; access flags
  u8int  flags;     //page granularity, size
  u8int  base_high; //last 8 bits of the base
}
  __attribute__ ((packed)) gdt_entry;


void idt_set_gate(u8int idx, u32int base, u16int sel, u8int flags);
void gdt_init_entry(int idx, u32int base, u32int limit, u8int access, 
		    u8int flags);

void init_idt();
void init_gdt();

#endif
