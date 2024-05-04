; Read some sectors from the boot disk using our disk_read function
[org 0x7c00]

mov [BOOT_DRIVE], dl        ; BIOS stores our boot drive in DL, so it's
                            ; best to remember this for later.

mov bp, 0x8000              ; Set our stack safely out of way,
mov sp, bp                  ; at 0x8000

mov bx, 0x9000              ; Load 2 sectors to 0x0000(es):0x9000(bx)
mov dh, 2                   ; from the boot disk.
                            ; We are reading the next 1 KB from the boot sector (which it is 
                            ; considered here as the disk), so there are no more than 2 sectors 
                            ; (2 blocks of 512 MB each or 1 KB) in total.
mov dl, [BOOT_DRIVE]
call disk_load

mov dx, [0x9000]            ; Print out the first loaded word, which
call print_hex              ; we expect to be 0xdada, stored
                            ; at address 0x9000

mov dx, [0x9000 + 512]      ; Also, print the first word from the
call print_hex              ; 2nd loaded sector: should be 0xface

jmp $

%include "../print_string.asm"
%include "../print_hex.asm"
%include "../disk_load.asm"

; Global variables
BOOT_DRIVE: db 0

; Bootsector padding
times 510-($-$$) db 0
dw 0xaa55


; We know that BIOS will load only the first 512-byte sector from the disk,
; so if we purposely add a few more sectors to our code by repeating some
; familiar numbers, we can prove to ourselves that we actually loaded those
; additional two sectors from the disk we booted from, as if they were
; in a real disk.
times 256 dw 0xdada ; 2 bytes 256 times = 512 bytes total
times 256 dw 0xface ; 2 bytes 256 times = 512 bytes total
times 256 dw 0xbebe