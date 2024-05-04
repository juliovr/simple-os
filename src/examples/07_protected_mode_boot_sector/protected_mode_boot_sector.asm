[org 0x7c00]

mov bp, 0x9000      ; Set the stack for 16-bit real mode.
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call switch_to_pm   ; We never return from here.

jmp $               ; Just as a safeguard.

%include "../print_string.asm"
%include "../gdt.asm"
%include "../print_string_pm.asm"
%include "../switch_to_pm.asm"

[bits 32]
; This is where we arrive after switching to and initializing protected mode.
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm    ; Use our 32-bit print routine.

    jmp $                   ; Hang.


MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Successfully landed in 32-bit Protected Mode", 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55
