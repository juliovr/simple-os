[org 0x7c00]                ; Tell the assembler where this code will be loaded

mov dx, 0x1fb6              ; Store the value to print in dx
call print_hex              ; Call the function


jmp $                       ; Hang


%include "../print_hex.asm"
%include "../print_string.asm"

;
; Padding and magic number
;
times 510-($-$$) db 0
dw 0xaa55