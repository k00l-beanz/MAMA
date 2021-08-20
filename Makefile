#
# Makefile for MPX

AS	= nasm
CC	= i386-elf-gcc
CFLAGS  = -Wall -Wextra -Werror -nostartfiles -nodefaultlibs -nostdlib -g -c
LD	= i386-elf-ld
LDFLAGS = -T link.ld
ASFLAGS = -f elf -g

OBJFILES =\
boot/loader.o\
kernel/kernel.o

MODULES =\
modules/modules.o

LIBS =\
lib/lib.o

# 1) declare your MODULES (see OBJFILES,LIBS above)

all: kernel.bin

.s.o:
	$(AS) $(ASFLAGS) -o $@ $<
.c.o: 
# 2) Modify this to add an include directory
	$(CC) $(CFLAGS) -I./include -I./modules -o $@ $< 

.PHONY : kernel/kernel.o
kernel/kernel.o:
	(cd kernel ; make)

# LIBS
.PHONY : lib/lib.o
lib/lib.o:
	(cd lib ; make)

# 3) Build your modules
.PHONY : modules/modules.o
modules/modules.o:
	(cd modules; make)

.PHONY : kernel.bin
kernel.bin: $(OBJFILES) $(LIBS) $(MODULES)
	$(LD) $(LDFLAGS) -o $@ $(OBJFILES) $(LIBS) $(MODULES)

.PHONY : kernel.img
kernel.img: kernel.bin
	dd if=/dev/zero of=pad bs=1 count=750
	cat boot/grub/stage1 boot/grub/stage2 pad kernel.bin > $@

# 6) Add a clean routine for your modules if you like
clean:
	(cd kernel ; make clean)
	(cd lib ; make clean)
	(cd modules ; make clean)
	rm -f $(OBJFILES) $(LIBS) kernel.bin kernel.img pad
