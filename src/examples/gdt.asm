; GDT
;
; Segment descriptor structure (GDT's entry):
; 
; Second 4 bytes:
;   - 31:24 : Base 31:24
;   - 23    : G
;   - 22    : D/B
;   - 21    : L
;   - 20    : AVL
;   - 19:16 : Segment Limit 19:16
;   - 15    : P
;   - 14:13 : DPL
;   - 12    : S
;   - 11:8  : Type
;   - 7:0   : Base 23:16
;
; First 4 bytes:
;   - 31:16 : Base Address 15:00
;   - 15:0  : Segment Limit 15:00
;
gdt_start:

gdt_null:           ; The mandatory first null descriptor
    dd 0x0          ; Define double word (i.e. 4 bytes, 32 bits)
    dd 0x0

gdt_code:           ; Code segment descriptor
    ; base = 0x0,   limit = 0xfffff
    ; 1st flags: (present)1 (privilege)00 (descriptor type)1 -> 0b1001
    ; type flags: (code)1 (conforming)0 (readable)1 (accessed)0 -> 0b1010
    ; 2nd flags: (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 -> 0b1100
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)

    db 0x0          ; Base (bits 16-23)
    db 10011010b    ; 1st flags, type flags
    db 11001111b    ; 2nd flags, Limit (bits 16-19)
    db 0x0          ; Base (bits 24-31)

gdt_data:           ; Data segment descriptor
    ; Same as code segment, except for the type flags:
    ; type flags: (code)0 (expand down)0 (writable)1 (accessed)0 -> 0b0010
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)

    db 0x0          ; Base (bits 16-23)
    db 10010010b    ; 1st flags, type flags
    db 11001111b    ; 2nd flags, Limit (bits 16-19)
    db 0x0          ; Base (bits 24-31)

gdt_end:            ; This label is useful to let the assembler calculate
                    ; the size of the GDT for the GDT descriptor.

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of our GDT, always minus one of the true size
    dd gdt_start                ; Start address of our GDT


 CODE_SEG equ gdt_code - gdt_start ; In this case, offset 0x8
 DATA_SEG equ gdt_data - gdt_start ; In this case, offset 0x10 (16)
