mov ah, 0x0e            ; Set BIOS in scrolling teletype mode (TTY mode)

; First attempt
mov al, the_secret
int 0x10

; Second attempt
mov al, [the_secret]
int 0x10

; Third attempt
mov bx, the_secret
add bx, 0x7c00          ; Adds the base address of the BIOS (0x7c00), which translates
mov al, [bx]            ; into the correct address of the data.
int 0x10

; Fourth attempt
mov al, [0x7c1d]        ; Same as previous attempt, but with the offset of the data (the_secret)
int 0x10                ; hard-coded (1d into the base address 0x7c00).

jmp $

the_secret:
    db 'X'

;
; Padding and magic number
;
times 510-($-$$) db 0
dw 0xaa55