loop:
    jmp loop            ; Just an infinite loop

times 510-($-$$) db 0   ; $: beginning of current line
                        ; $$: beginning of current section
                        ; So, doing $-$$ calculates how far we are in the program.
                        ; Using 510 times minus how far we are to fill with 0's.
                        ; The next line fills the remaining 2 bytes to complete the 512 boot sector.

dw 0xaa55               ; Magic number so BIOS knows it is a boot sector and not just data on disk.