; Stage 1
; Version 1
; Author = AR-DEV-1
; This is licensed under GPL 3.0

BITS 16
ORG 0x7C00

start:
    ; Clear screen
    xor ax, ax
    mov es, ax
    mov di, 0x8000
    mov cx, 32000
    rep stosw

    ; Check CPU architecture
    mov ax, 0x5307           ; CPUID function to get processor info
    cpuid
    mov ebx, edx              ; Store vendor ID or use other flags
    ; Use EBX to check if it's 64-bit capable or 32-bit only

    ; If 64-bit, jump to a separate code path
    ; (e.g., 64-bit mode transition requires special handling)

    ; Enable A20 gate (necessary for Protected Mode)
    call enable_a20

    ; Load Stage 2
    mov ah, 0x02         ; BIOS interrupt for disk read
    mov al, 5            ; Read 5 sectors
    mov ch, 0            ; Cylinder 0
    mov cl, 3            ; Sector 3 (after FAT32 boot sector)
    mov dh, 0            ; Head 0
    mov dl, 0x80         ; First hard drive
    mov bx, 0x9000       ; Load address for Stage 2
    int 0x13
    jc disk_error        ; Jump if error

    ; Switch to Protected Mode (32-bit)
    cli                  ; Disable interrupts
    lgdt [gdt_descriptor]; Load Global Descriptor Table
    mov eax, cr0
    or eax, 1
    mov cr0, eax         ; Enable Protected Mode
    jmp 0x08:protected_mode_start

disk_error:
    ; Handle disk error (display message)
    mov ah, 0x0E
    mov al, 'E'
    int 0x10
    hlt

enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

gdt:
    dq 0
    dw 0xFFFF, 0x0000, 0x9A00, 0x00CF  ; Code segment
    dw 0xFFFF, 0x0000, 0x9200, 0x00CF  ; Data segment

gdt_descriptor:
    dw 23             ; Limit (size of GDT - 1)
    dd gdt            ; Address of GDT

protected_mode_start:
    mov ax, 0x10      ; Data segment
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x7C00   ; Stack pointer

    ; If it's 64-bit mode, jump to 64-bit code
    ; (Handling 64-bit mode will happen here, after checking flags)

    ; Jump to Stage 2
    jmp 0x9000

times 510 - ($ - $$) db 0
dw 0xAA55
