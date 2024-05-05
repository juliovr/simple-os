;
; Hex value should be in dx register
;
print_hex:
    ; Digit 1
    mov ax, dx              ; Copy hex value to ax
    shr ax, 12              ; Shift to the last hex digit
    and ax, 0xF             ; Just leave the last 4 binary digits (the last hex digit)

    cmp ax, 0x9             ; Test if the number
    jg hex_digit_1          ; is greater than 10

    add ax, 0x30            ; If the digit is between 0-9, add the '0' ascii value
    jmp replace_digit_1

hex_digit_1:
    add ax, 0x41 - 0xA      ; Get ascii value of 10-16 hex.
                            ; ax holds the value to print and 0x41 - 0xA 
                            ; adds the offset to the correct ascii value.
                            ; 0x41 is 'A', so we need to substract 10 (0xA)
                            ; to make 10 the 0 offset.

replace_digit_1:
    mov [HEX_OUT + 2], al   ; Replace the calculated digit in the correct hex-string position


    ; Digit 2
    mov ax, dx              ; Copy hex value to ax
    shr ax, 8               ; Shift to the last hex digit
    and ax, 0xF             ; Just leave the last 4 binary digits (the last hex digit)
    
    cmp ax, 0x9             ; Test if the number
    jg hex_digit_2          ; is greater than 10

    add ax, 0x30            ; If the digit is between 0-9, add the '0' ascii value
    jmp replace_digit_2

hex_digit_2:
    add ax, 0x41 - 0xA

replace_digit_2:
    mov [HEX_OUT + 3], al   ; Replace the calculated digit in the correct hex-string position


    ; Digit 3
    mov ax, dx              ; Copy hex value to ax
    shr ax, 4               ; Shift to the last hex digit
    and ax, 0xF             ; Just leave the last 4 binary digits (the last hex digit)

    cmp ax, 0x9             ; Test if the number
    jg  hex_digit_3         ; is greater than 10

    add ax, 0x30            ; If the digit is between 0-9, add the '0' ascii value
    jmp replace_digit_3

hex_digit_3:
    add ax, 0x41 - 0xA

replace_digit_3:
    mov [HEX_OUT + 4], al   ; Replace the calculated digit in the correct hex-string position


    ; Digit 4
    mov ax, dx              ; Copy hex value to ax
    and ax, 0xF             ; Just leave the last 4 binary digits (the last hex digit)

    cmp ax, 0x9             ; Test if the number
    jg hex_digit_4          ; is greater than 10

    add ax, 0x30            ; If the digit is between 0-9, add the '0' ascii value
    jmp replace_digit_4

hex_digit_4:
    add ax, 0x41 - 0xA

replace_digit_4:
    mov [HEX_OUT + 5], al   ; Replace the calculated digit in the correct hex-string position

    

    mov bx, HEX_OUT         ; Print the string pointed to
    call print_string       ; by bx
    ret


; Global variables
HEX_OUT:
    db '0x0000', 0
