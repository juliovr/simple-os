;
; Hex value should be in dx register
;
print_hex:
    mov bx, HEX_OUT         ; Print the string pointed to
    call print_string       ; by bx
    ret


; Global variables
HEX_OUT:
    db '0x0000', 0
