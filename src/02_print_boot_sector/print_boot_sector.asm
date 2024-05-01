mov ah, 0x0e            ; Set BIOS in scrolling teletype mode (TTY mode)

mov al, 'H'
int 0x10                ; Raise interrumpt 0x10, which print the character in al, following the ah mode.
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
mov al, 'l'
int 0x10
mov al, 'o'
int 0x10

jmp $                   ; Jump to the beginning of the current line, which means, an infinite loop.

;
; Padding and magic number
;
times 510-($-$$) db 0
dw 0xaa55