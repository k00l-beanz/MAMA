[BITS 32]

global start
global magic
global mbd

;; kernel main
extern kmain

;; multiboot header
;; see GRUB docs for details
MODULEALIGN equ 1<<0			; align modules on page boundaries
MEMINFO     equ 1<<1			; provide memory map
FLAGS       equ MODULEALIGN | MEMINFO	; multiboot enable
MAGIC       equ 0x1badb002		; used to check boot success
CHECKSUM    equ -(MAGIC + FLAGS)

section .text
	
align 4
multiboot_header:
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

;; reserve initial stack space
STACKSIZE equ 0x4000

;; kernel entry point
start:
	mov esp, stack + STACKSIZE
	mov [magic], eax
	mov [mdb], ebx
	call kmain
	cli
.hang:
	hlt
	jmp .hang

section .bss

align 4
stack:	resb STACKSIZE	; reserve stack on doubleword boundary
magic:	resb 1
mdb:	resb 1
