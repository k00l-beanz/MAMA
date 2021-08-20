
  ;; ----- irq.s -----

  ;; Description..: Interrupt handler stubs. All call C routines
  ;; 	to do the real work. Also, default system call handler
  ;; 	located at the bottom.


[GLOBAL divide_error]
[GLOBAL debug]
[GLOBAL nmi]
[GLOBAL breakpoint]
[GLOBAL overflow]
[GLOBAL bounds]
[GLOBAL invalid_op]
[GLOBAL device_not_available]
[GLOBAL double_fault]
[GLOBAL coprocessor_segment]
[GLOBAL invalid_tss]
[GLOBAL segment_not_present]
[GLOBAL stack_segment]
[GLOBAL general_protection]
[GLOBAL page_fault]
[GLOBAL reserved]
[GLOBAL coprocessor]
[GLOBAL rtc_isr]

;; Names of the C handlers
extern do_divide_error
extern do_debug
extern do_nmi
extern do_breakpoint
extern do_overflow
extern do_bounds
extern do_invalid_op
extern do_device_not_available
extern do_double_fault
extern do_coprocessor_segment
extern do_invalid_tss
extern do_segment_not_present
extern do_stack_segment
extern do_general_protection
extern do_page_fault
extern do_reserved
extern do_coprocessor

; RTC interrupt handler
; Tells the slave PIC to ignore
; interrupts from the RTC
rtc_isr:
	cli
	push ax
	
	; Tell the PIC this is EOI
	; This really should be done
	; at the RTC level -- but this is
	; okay for now...
	mov al, 0x20
	out 0xA0, al
	
	pop ax
	sti
	iret

divide_error:
	call do_divide_error
	iret
debug:
	call do_debug
	iret
nmi:
	call do_nmi
	iret
breakpoint:
	call do_breakpoint
	iret
overflow:
	call do_overflow
	iret
bounds:
	call do_bounds
	iret
invalid_op:
	call do_invalid_op
	iret
device_not_available:
	call do_device_not_available
	iret
double_fault:
	call do_double_fault
	iret
coprocessor_segment:
	call do_coprocessor_segment
	iret
invalid_tss:
	call do_invalid_tss
	iret
segment_not_present:
	call do_segment_not_present
	iret
stack_segment:
	call do_stack_segment
	iret
general_protection:
	call do_general_protection
	iret
page_fault:
	call do_page_fault
	iret
reserved:
	call do_reserved
	iret
coprocessor:
	call do_coprocessor
	iret

;;; System call interrupt handler. Pushes all the x86 registers
;;; onto the stack followed by ds,es,fs,gs (see context structure).
;;; Pushes esp last, which the function can cast to a context and
;;; access the registers. The C handler returns the address of the
;;; new processes stack top/pointer.
sys_call_isr:
	iret
