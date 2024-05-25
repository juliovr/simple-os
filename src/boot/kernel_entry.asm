; This file calls the main function on kernel.c.
; It is just serve as an entry point to the kernel, because if we call
; the main function directly to their loaded address in memory, we are
; conditioning that the main function is at the top of the file (i.e. the
; first instruction in that compilation unit), so with this file the main
; function is executed as it is, as a function.
; In this file is mandatory to not have any more code.
bits 32
extern bootmain

kernel_entry:
    call bootmain

    jmp $                   ; Hang (as a safeguard).


;
; TODO: move to another file!!!
;

; Default isr handler to fill the 256 idt entries
global isr_generic_handler
isr_generic_handler:
    jmp $


extern isr_handler
extern irq_handler

isr_common_stub:
    ; 1. Save CPU state
    pusha               ; Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov ax, ds          ; Lower 16-bits
    push eax            ;   to save the data segment descriptor
    mov ax, 0x10        ; Load kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; 2. Call ISR handler in idt.c
    push esp            ; Pass pointer to stack to C, so we can access all the pushed information
    call isr_handler
    add esp, 4          ; Clean up pointer parameter passed to C function by restoring the stack pointer.

    ; 3. Restore CPU state
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8          ; Cleans up the pushed error code and ISR number.
    sti                 ; Turn on interrupt flag
    iret                ; Pops cs, eip, eflags, ss and esp


irq_common_stub:
    ; 1. Save CPU state
    pusha               ; Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov ax, ds          ; Lower 16-bits
    push eax            ;   to save the data segment descriptor
    mov ax, 0x10        ; Load kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; 2. Call ISR handler in idt.c
    push esp            ; Pass pointer to stack to C, so we can access all the pushed information
    call irq_handler
    add esp, 4          ; Clean up pointer parameter passed to C function by restoring the stack pointer.

    ; 3. Restore CPU state
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8          ; Cleans up the pushed error code and ISR number.
    sti                 ; Turn on interrupt flag
    iret                ; Pops cs, eip, eflags, ss and esp



; ISRs
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

; IRQs
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15


;
; ISRs
;

; 0: Divide By Zero Exception
isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

; 1: Debug Exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

; 2: Non-maskable Interrupt Exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

; 3: Breakpoint Exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

; 4: Overflow Exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

; 5: Bound Range Exceeded Exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

; 6: Invalid Opcode Exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

; 7: Device Not Available Exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

; 8: Double Fault Exception (with Error Code)
isr8:
    cli
    push byte 8
    jmp isr_common_stub

; 9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

; 10: Invalid TSS Exception (with Error Code)
isr10:
    cli
    push byte 10
    jmp isr_common_stub

; 11: Segment Not Present Exception (with Error Code)
isr11:
    cli
    push byte 11
    jmp isr_common_stub

; 12: Stack-Segment Fault Exception (with Error Code)
isr12:
    cli
    push byte 12
    jmp isr_common_stub

; 13: General Protection Fault Exception (with Error Code)
isr13:
    cli
    push byte 13
    jmp isr_common_stub

; 14: Page Fault Exception (with Error Code)
isr14:
    cli
    push byte 14
    jmp isr_common_stub

; 15: Reserved
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

; 16: x87 Floating-Point Exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

; 17: Alignment Check Exception (with Error Code)
isr17:
    cli
    push byte 17
    jmp isr_common_stub

; 18: Machine Check
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

; 19: Reserved
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

; 20: Reserved
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

; 21: Reserved
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

; 22: Reserved
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub

; 23: Reserved
isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub

; 24: Reserved
isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub

; 25: Reserved
isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub

; 26: Reserved
isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub

; 27: Reserved
isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

; 28: Reserved
isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

; 29: Reserved
isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

; 30: Reserved
isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

; 31: Reserved
isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub



;
; IRQs
;

irq0:
    cli
    push byte 0
    push byte 32
    jmp irq_common_stub

irq1:
    cli
    push byte 0
    push byte 33
    jmp irq_common_stub

irq2:
    cli
    push byte 0
    push byte 34
    jmp irq_common_stub

irq3:
    cli
    push byte 0
    push byte 35
    jmp irq_common_stub

irq4:
    cli
    push byte 0
    push byte 36
    jmp irq_common_stub

irq5:
    cli
    push byte 0
    push byte 37
    jmp irq_common_stub

irq6:
    cli
    push byte 0
    push byte 38
    jmp irq_common_stub

irq7:
    cli
    push byte 0
    push byte 39
    jmp irq_common_stub

irq8:
    cli
    push byte 0
    push byte 40
    jmp irq_common_stub

irq9:
    cli
    push byte 0
    push byte 41
    jmp irq_common_stub

irq10:
    cli
    push byte 0
    push byte 42
    jmp irq_common_stub

irq11:
    cli
    push byte 0
    push byte 43
    jmp irq_common_stub

irq12:
    cli
    push byte 0
    push byte 44
    jmp irq_common_stub

irq13:
    cli
    push byte 0
    push byte 45
    jmp irq_common_stub

irq14:
    cli
    push byte 0
    push byte 46
    jmp irq_common_stub

irq15:
    cli
    push byte 0
    push byte 47
    jmp irq_common_stub
