;
; Address of the string to print should be in 'bx' register
;
print_string:
    pusha

    mov ah, 0x0e                ; BIOS TTY output

begin_loop_print_string:
    mov dx, [bx]                ; Get the character of the string pointed to by bx

    test dl, dl                 ; Is char 0?
    je end_loop_print_string    ; If it is 0, end the loop

    mov al, dl                  ; Move the character to al 
    int 0x10                    ; to print the value.

    add bx, 1                   ; Increment char pointer

    jmp begin_loop_print_string

end_loop_print_string:
    ; Print line feed (LF)
    mov al, 0xa
    int 0x10

    ; Print carriage return (CR)
    mov al, 0xd
    int 0x10

    popa
    ret
