; Load DH sectors to es:bx from drive dl
disk_load:
    push dx             ; Store dx on stack so later we can recall
                        ; how many sectors were request to be read,
                        ; even if it is altered in the meantime.
    mov ah, 0x02        ; BIOS read sector function
    mov al, dh          ; Read DH sectors
    mov ch, 0x00        ; Select cylinder 0
    mov dh, 0x00        ; Select head 0
    mov cl, 0x02        ; Start reading from second sector 
                        ; (i.e. after the boot sector)
    int 0x13            ; BIOS interrupt for low level disk services
                        ; (the operation is given on ah, in this case, 0x02)
    
    jc disk_error       ; Jump if error (i.e. carry flag set)

    pop dx              ; Restore dx from the stack
    cmp dh, al          ; if al (sectors read) != dh (sectors expected)
    jne disk_error      ;     display error message
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $

; Variables
DISK_ERROR_MSG:
    db "Disk read error!", 0

