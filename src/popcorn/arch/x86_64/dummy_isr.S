;;; Interrupt service routine - common code.
        section .text

        global idt_flush

idt_flush:
	mov rax, rdi
	lidt [rax]
	ret
;;; two macros for the two basic interrupt skeleton types - one which pushes an error code, one that does not.
;;; this ensures we've got a well-formed stack alignment when we jump to the common stub handler.
%macro ISR_NOERRCODE 1
	global isr%1
        xchg bx, bx
        isr%1:
	cli
        mov rdi, %1
        mov rsi, %1
	  jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
	global isr%1
        isr%1:
	cli
        xchg bx, bx
        mov rdi, %1
        pop rsi
	  jmp isr_common_stub	; The error code is already on the stack, so isr_common stub still pops 2 values off
%endmacro

%macro IRQ 2
	global irq%1
        irq%1:
	cli
        xchg bx, bx
	  push byte 0
	  push byte %2
	  jmp irq_common_stub
%endmacro

;;; Define 32 interrupt handlers.

ISR_NOERRCODE 0			; appropriately, divide by 0
ISR_NOERRCODE 1			; Debug exception
ISR_NOERRCODE 2			; NMI
ISR_NOERRCODE 3			; Breakpoint exception
ISR_NOERRCODE 4			; Into detected overflow
ISR_NOERRCODE 5			; Out of bounds exception
ISR_NOERRCODE 6			; Invalid opcode
ISR_NOERRCODE 7			; Coprocessor not present
ISR_ERRCODE   8			; double fault
ISR_NOERRCODE 9			; Coprocessor segment overrun
ISR_ERRCODE   10		; Bad TSS
ISR_ERRCODE   11		; Segment not present
ISR_ERRCODE   12		; Stack fault
ISR_ERRCODE   13		; General protection fault
ISR_ERRCODE   14		; Page fault
ISR_NOERRCODE 15		; Unknown interrupt
ISR_NOERRCODE 16		; Coprocessor fault
ISR_NOERRCODE 17		; Alignment check exception
ISR_NOERRCODE 18		; Machine check exception
ISR_NOERRCODE 19		; Interrupts 19-31 are reserved by Intel.
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47


extern isr_handler		; This lives in C land

isr_common_stub:
        xor rax, rax
	mov ax, ds
	push rax		; data segment descriptor

        

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call isr_handler
        xchg bx, bx
	pop rax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	iretq

extern irq_handler		; Defined in interrupts.c
irq_common_stub:
	mov ax, ds		; save data segment descriptor
	push rax
	mov ax, 0x10 		; load kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call irq_handler
	pop rbx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx

	add rsp, 8
	sti
	iretq
