.set MB_MAGIC, 0x1badb002
.set MB_FLAGS, 0x00000003

.section .multiboot,"a"
.balign 4
.long MB_MAGIC
.long MB_FLAGS
.long -(MB_MAGIC + MB_FLAGS)

.section .bss,"aw",@nobits
.balign 16
.global stack_bottom, stack_top
stack_bottom:
.skip 16384
stack_top:

.section .text
.code32
.global _start
.type _start, @function
.extern kernel_main
_start:
    cli
    cld
    /* BIOS raw loading also uses this entry; zero BSS without using a stack. */
    movl %eax, %esi
    movl %ebx, %ebp
    movl $__bss_start, %edi
    movl $__bss_end, %ecx
    subl %edi, %ecx
    xorl %eax, %eax
    rep stosb
    movl %esi, %eax
    movl %ebp, %ebx
    movl $stack_top, %esp
    xorl %ebp, %ebp
    /* Align ESP to 16 bytes before CALL and pass the Multiboot registers. */
    subl $8, %esp
    pushl %ebx
    pushl %eax
    call kernel_main
.Lhalt:
    cli
    hlt
    jmp .Lhalt
.size _start, . - _start

.section .note.GNU-stack,"",@progbits
