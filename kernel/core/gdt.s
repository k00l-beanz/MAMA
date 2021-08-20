
  ;; ----- gdt.s -----

  ;; Description..: Routines used for installing the initial
  ;; 	global descriptor table..


[GLOBAL write_gdt_ptr]

;;; Installs the initial global descriptor table
;;; using the lgdt (load gdt) instruction
write_gdt_ptr:
	mov eax,[esp+4]		;get pointer to gdt
	lgdt [eax]		;load the new gdt pointer

	mov ax,0x10		;offset to data segment
	mov ds,ax		;load data segment
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	jmp 0x08:.flush_gdt	;jump to code segment and reset regs

.flush_gdt:
	ret
