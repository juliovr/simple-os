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

times 510-($-$$) db 0   ; $: beginning of current line
                        ; $$: beginning of current section
                        ; So, doing $-$$ calculates how far we are in the program.
                        ; Using 510 times minus how far we are to fill with 0's.
                        ; The next line fills the remaining 2 bytes to complete the 512 boot sector.

dw 0xaa55               ; Magic number so BIOS knows it is a boot sector and not just data on disk.