#include "platform.h"

#define COM1 0x3f8
#define WIDTH 80u
#define HEIGHT 25u
static volatile u16 *const video = (volatile u16 *)0xb8000;
static u32 row, column;
static int filter_letters;

void console_filter_toggle(void)
{
    /* TODO: implement this lab; consult its task sheet. */
    /* Keep the toggle state disabled until implemented. */
}

void console_init(void)
{
    outb(COM1 + 1, 0);
    outb(COM1 + 3, 0x80);
    outb(COM1, 1);
    outb(COM1 + 1, 0);
    outb(COM1 + 3, 3);
    outb(COM1 + 2, 0xc7);
    outb(COM1 + 4, 0x0b);
    row = column = 0;
    for (u32 i = 0; i < WIDTH * HEIGHT; ++i)
        video[i] = 0x0720;
}

static void serial_putc(char c)
{
    /* A missing UART must not prevent the VGA console from progressing. */
    for (u32 tries = 0; tries < 100000; ++tries) {
        if (inb(COM1 + 5) & 0x20) {
            outb(COM1, (u8)c);
            return;
        }
    }
}

static void putc(char c)
{
    if (filter_letters && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
        c = '*';
    if (c == '\n') {
        serial_putc('\r');
        column = 0;
        ++row;
    } else {
        video[row * WIDTH + column] = 0x0f00 | (u8)c;
        if (++column == WIDTH) {
            column = 0;
            ++row;
        }
    }
    serial_putc(c);
    if (row == HEIGHT) {
        for (u32 i = 0; i < (HEIGHT - 1) * WIDTH; ++i)
            video[i] = video[i + WIDTH];
        for (u32 i = (HEIGHT - 1) * WIDTH; i < HEIGHT * WIDTH; ++i)
            video[i] = 0x0720;
        row = HEIGHT - 1;
    }
}

void console_puts(const char *text)
{
    while (*text)
        putc(*text++);
}

void console_hex(u32 value)
{
    static const char digits[] = "0123456789abcdef";
    console_puts("0x");
    for (int shift = 28; shift >= 0; shift -= 4)
        putc(digits[(value >> shift) & 15]);
}

void console_uint(u32 value)
{
    char digits[10];
    u32 count = 0;
    do {
        digits[count++] = (char)('0' + value % 10);
        value /= 10;
    } while (value);
    while (count)
        putc(digits[--count]);
}
