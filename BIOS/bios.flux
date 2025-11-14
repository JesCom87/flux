; Simple BIOS bootloader structure
[org 0x7c00]

start:
    cli                          ; Disable interrupts
    mov ax, 0x0000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti                          ; Enable interrupts

    mov ah, 0x0e                 ; BIOS print character function
    mov al, 'H'
    int 0x10
    
    jmp $                        ; Infinite loop

times 510 - ($ - $$) db 0       ; Pad to 510 bytes
dw 0xaa55                        ; Boot signature