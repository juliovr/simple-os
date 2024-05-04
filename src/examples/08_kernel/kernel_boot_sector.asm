[org 0x7c00]

KERNEL_OFFSET equ 0x1000    ; Memory offset of which the kernel is located
                            ; (specified when was linked with -Ttext arg).


mov [BOOT_DRIVE], dl        ; BIOS stores our boot drive in DL, so it's
                            ; best to remember this for later.

mov bp, 0x9000              ; Set the stack for 16-bit real mode.
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call load_kernel            ; Load our kernel into memory.

call switch_to_pm           ; We never return from here.

jmp $                       ; Just as a safeguard.

%include "../print_string.asm"
%include "../disk_load.asm"
%include "../gdt.asm"
%include "../print_string_pm.asm"
%include "../switch_to_pm.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print_string

    ; Set-up parameters for our disk_load routine.
    mov bx, KERNEL_OFFSET   ; Load into address KERNEL_OFFSET.
    mov dh, 9               ; We load the first 9 sectors (excluding 
    mov dl, [BOOT_DRIVE]    ; the boot sector) from the boot disk (i.e. our
    call disk_load          ; kernel code) to address KERNEL_OFFSET.
                            ; Attempting to read more sector than the os_image
                            ; will trigger a disk error because the are
                            ; no more sectors (the file is not that big).

    ret


[bits 32]
; This is where we arrive after switching to and initializing protected mode.
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm    ; Use our 32-bit print routine.

    ; Executing 'call KERNEL_OFFSET' works because the first function in kernel.c 
    ; is the main routine, but if we add more code on top of that, that 
    ; new code will be executed because it is at the KERNEL_OFFSET address. 
    ; A better way is to call the function directly, in which the function
    ; address must be resolve by the linker (which means the assembly section 
    ; that calls the kernel entry-point should be assembled as 
    ; object file and not binary).
    ; So, we execute the code at KERNEL_OFFSET (which resides on kernel_entry.asm)
    ; and that asm calls the main function on kernel.c.
    call KERNEL_OFFSET      ; Execute code at this address.

    jmp $                   ; Hang (as a safeguard).


; Global variables
BOOT_DRIVE      db 0
MSG_REAL_MODE   db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE   db "Successfully landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55
