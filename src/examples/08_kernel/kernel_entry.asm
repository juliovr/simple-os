; This file calls the main function on kernel.c.
; It is just serve as an entry point to the kernel, because if we call
; the main function directly to their loaded address in memory, we are
; conditioning that the main function is at the top of the file (i.e. the
; first instruction in that compilation unit), so with this file the main
; function is executed as it is, as a function.
; In this file is mandatory to not have any more code.
[bits 32]
[extern bootmain]
kernel_entry:
    call bootmain

    jmp $                   ; Hang (as a safeguard).