/* GNU assembler syntax; a real BIOS boot sector, loaded at 0000:7c00. */
.code16
.section .text
.global _start
_start:
    ljmp $0x0000, $start
start:
    cli
    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw $0x7c00, %sp
    sti
    cld
    movb %dl, drive
    movw $message, %si
    call puts
    /* Relocate the 512-byte boot sector, as in the course's bootsect. */
    movw $0x9000, %ax
    movw %ax, %es
    movw $0x7c00, %si
    xorw %di, %di
    movw $256, %cx
    rep movsw
    ljmp $0x9000, $(relocated - _start)
relocated:
    movw $0x9000, %ax
    movw %ax, %ds
    movw %ax, %es
    movw $3, %di
retry:
    xorw %dx, %dx
    movb (drive - _start), %dl
    movw $0x0200, %bx
    movw $0x0002, %cx
    movw $0x0204, %ax
    int $0x13
    jnc loaded
    xorw %ax, %ax
    int $0x13
    decw %di
    jnz retry
    /* E9 is a QEMU debug console, not a BIOS service. */
    movb $'!', %al
    outb %al, $0xe9
hang:
    cli
    hlt
    jmp hang
loaded:
    movb (drive - _start), %dl
    ljmp $0x9020, $0
puts:
    lodsb
    testb %al, %al
    jz done
    outb %al, $0xe9
    movb $0x0e, %ah
    xorw %bx, %bx
    int $0x10
    jmp puts
done:
    ret
drive: .byte 0
message: .asciz "[BIOS] bootsect at 7c00\r\n"
.org 510
.word 0xaa55
.section .note.GNU-stack,"",@progbits
