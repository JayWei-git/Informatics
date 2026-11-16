#ifndef OSLAB_PLATFORM_H
#define OSLAB_PLATFORM_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
_Static_assert(sizeof(u32) == 4, "requires a 32-bit word");
_Static_assert(sizeof(void *) == 4, "compile with -m32");

static inline void outb(u16 port, u8 value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline u8 inb(u16 port)
{
    u8 value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline void outl(u16 port, u32 value)
{
    __asm__ volatile ("outl %0, %1" : : "a"(value), "Nd"(port));
}

void console_init(void);
void console_puts(const char *text);
void console_hex(u32 value);
void console_uint(u32 value);
__attribute__((noreturn)) void kernel_main(u32 magic, u32 info_address);

#endif
