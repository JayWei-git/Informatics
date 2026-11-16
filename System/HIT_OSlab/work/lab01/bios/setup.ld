OUTPUT_FORMAT(binary)
OUTPUT_ARCH(i386)
ENTRY(setup_start)
SECTIONS {
    . = 0x90200;
    .text : { *(.text) }
    /DISCARD/ : { *(.note*) *(.debug*) *(.comment) }
}
