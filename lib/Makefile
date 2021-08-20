#
# Makefile for MPX/Linux libs

AS      = nasm
CC      = i386-elf-gcc
CFLAGS  = -Wall -Wextra -Werror -nostartfiles -nostdlib -nodefaultlibs -ffreestanding -g -c
LD      = i386-elf-ld
LDFLAGS = 
ASFLAGS = -f elf -g

OBJFILES =\
string.o

.c.s:
	$(CC) $(CFLAGS) -S -o $@ $<
.s.o:
	$(AS) $(ASFLAGS) -c -o $@ $<
.c.o:
	$(CC) $(CFLAGS) -I../include -o $@ $<

all: lib.o

lib.o: $(OBJFILES)
	$(LD) -r -o lib.o $(OBJFILES)

clean:
	rm $(OBJFILES)
