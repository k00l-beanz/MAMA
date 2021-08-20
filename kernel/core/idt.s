
  ;; ----- idt.s -----

  ;; Description..: Routine for install the initial
  ;; 	interrupt descriptor table.

	
[GLOBAL write_idt_ptr]

;;; Installs the interrupt descriptor table
;;; using the lidt (load IDT) instruction 
write_idt_ptr:
	mov eax,[esp+4] 	; get idt pointer
	lidt [eax]		; load idt pointer
	ret
