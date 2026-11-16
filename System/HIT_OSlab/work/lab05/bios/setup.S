#include "layout.inc"
.code16
.section .text
.global setup_start
setup_start:
    cli
    movw $0x9020, %ax
    movw %ax, %ds
    movw $0x8000, %ax
    movw %ax, %ss
    movw $0xf000, %sp
    sti
    cld
    movb %dl, (drive - setup_start)
    movw $(message - setup_start), %si
    call puts
    /* A small Multiboot-compatible memory-info prefix for our kernel. */
    xorw %ax, %ax
    movw %ax, %es
    movl $1, %es:0x9500
    int $0x12
    movzwl %ax, %eax
    movl %eax, %es:0x9504
    movw $0x8800, %ax
    int $0x15
    jc error
    movzwl %ax, %eax
    movl %eax, %es:0x9508
    movw $(memory_msg - setup_start), %si
    call puts
    movw %es:0x9508, %ax
    call hex16
    movw $(newline - setup_start), %si
    call puts
    /* Read one sector per request, avoiding track and 64 KiB crossings. */
    movw $0x1000, %ax
    movw %ax, %es
    movw $5, %di
    movw $KERNEL_SECTORS, %bp
read_next:
    movw %di, %ax
    xorw %dx, %dx
    movw $18, %si
    divw %si
    movb %dl, %cl
    incb %cl
    xorw %dx, %dx
    movw $2, %si
    divw %si
    movb %al, %ch
    movb %dl, %dh
    movb (drive - setup_start), %dl
    xorw %bx, %bx
    movw $0x0201, %ax
    int $0x13
    jc error
    movw %es, %ax
    addw $0x20, %ax
    movw %ax, %es
    incw %di
    decw %bp
    jnz read_next
    movw $(disk_msg - setup_start), %si
    call puts
    /* BIOS services are finished; establish our own protected-mode state. */
    cli
    inb $0x92, %al
    andb $0xfe, %al
    orb $2, %al
    outb %al, $0x92
    lgdt (gdt_pointer - setup_start)
    lidt (idt_pointer - setup_start)
    movl %cr0, %eax
    orl $1, %eax
    movl %eax, %cr0
    ljmpl $0x08, $protected_entry
error:
    movw $(error_msg - setup_start), %si
    call puts
1:  cli
    hlt
    jmp 1b
puts:
    lodsb
    testb %al, %al
    jz 2f
    outb %al, $0xe9
    movb $0x0e, %ah
    xorw %bx, %bx
    int $0x10
    jmp puts
2:  ret
hex16:
    movw %ax, %dx
    movw $4, %cx
3:  rolw $4, %dx
    movb %dl, %al
    andb $15, %al
    addb $'0', %al
    cmpb $'9', %al
    jbe 4f
    addb $7, %al
4:  outb %al, $0xe9
    pushw %dx
    pushw %cx
    movb $0x0e, %ah
    xorw %bx, %bx
    int $0x10
    popw %cx
    popw %dx
    loop 3b
    ret
.balign 8
gdt:
    .quad 0
    .quad 0x00cf9a000000ffff
    .quad 0x00cf92000000ffff
gdt_end:
gdt_pointer:
    .word gdt_end - gdt - 1
    .long gdt
idt_pointer:
    .word 0
    .long 0
drive: .byte 0
message: .asciz "[BIOS] Now we are in SETUP\r\n"
memory_msg: .asciz "[BIOS] extended_KiB(hex)="
newline: .asciz "\r\n"
disk_msg: .asciz "[BIOS] kernel sectors loaded\r\n"
error_msg: .asciz "[FAIL] BIOS disk or memory service\r\n"
.code32
protected_entry:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    movl $0x8f000, %esp
    cld
    movl $0x10000, %esi
    movl $0x100000, %edi
    movl $KERNEL_BYTES, %ecx
    rep movsb
    /* Same C handoff contract as the quick Multiboot path. */
    movl $0x2badb002, %eax
    movl $0x9500, %ebx
    movl $KERNEL_ENTRY, %edx
    jmp *%edx
.section .note.GNU-stack,"",@progbits
