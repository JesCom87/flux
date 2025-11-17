; Simple x86 Bootloader
; This is a minimal 512-byte bootloader for real mode

[BITS 16]
[ORG 0x7C00]

start:
    ; Disable interrupts
    cli
    
    ; Set up stack
    mov ax, 0x0000
    mov ss, ax
    mov sp, 0x7C00
    
    ; Initialize registers
    mov ax, 0x0000
    mov ds, ax
    mov es, ax
    
    ; Display a message
    mov si, message
    call print_string
    
    ; Halt the system
    hlt

print_string:
    lodsb
    cmp al, 0
    je .done
    mov ah, 0x0E
    mov bh, 0x00
    mov bl, 0x07
    int 0x10
    jmp print_string
.done:
    ret

message:
    db "Bootloader loaded!", 0

; Pad to 510 bytes and add boot signature
times 510-($-$$) db 0
dw 0xAA55