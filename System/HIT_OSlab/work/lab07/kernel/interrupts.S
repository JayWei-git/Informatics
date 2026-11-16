.code32
.section .text
.extern interrupt_dispatch
.macro ISR n
.global isr\n
isr\n:
.if (\n != 8) && (\n != 10) && (\n != 11) && (\n != 12) && (\n != 13) && (\n != 14) && (\n != 17) && (\n != 21) && (\n != 29) && (\n != 30)
    pushl $0
.endif
    pushl $\n
    jmp interrupt_common
.endm
.irp n,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,128,129
ISR \n
.endr
interrupt_common:
    cld
    pusha
    push %ds
    push %es
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movl %esp, %ebx
    andl $-16, %esp
    subl $12, %esp
    pushl %ebx
    call interrupt_dispatch
    movl %eax, %esp
    pop %es
    pop %ds
    popa
    addl $8, %esp
    iret
.global load_gdt
load_gdt:
    movl 4(%esp), %eax
    lgdt (%eax)
    ljmp $8, $1f
1:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    movw %ax, %fs
    movw %ax, %gs
    ret
.section .rodata
.global isr_table
isr_table:
.irp n,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,128,129
    .long isr\n
.endr
.section .note.GNU-stack,"",@progbits
