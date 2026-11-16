#include "platform.h"
#include "runtime.h"

/* Only the fixed, initial part of the Multiboot v1 information structure. */
struct multiboot_info {
    u32 flags;
    u32 mem_lower;
    u32 mem_upper;
    u32 boot_device;
    u32 cmdline;
};

static volatile u32 bss_probe;
static volatile u32 data_probe = 0x11223344;

static int has_token(const char *text, const char *token)
{
    while (*text) {
        while (*text == ' ')
            ++text;
        const char *a = text, *b = token;
        while (*a && *b && *a == *b) {
            ++a;
            ++b;
        }
        if (!*b && (!*a || *a == ' '))
            return 1;
        while (*text && *text != ' ')
            ++text;
    }
    return 0;
}

void kernel_main(u32 magic, u32 info_address)
{
    console_init();
    console_puts("HIT OS Lab | Arch native | lab01\n");
    console_puts("[boot] entered 32-bit C kernel\n");
    console_puts("[boot] magic=");
    console_hex(magic);
    console_puts(" info=");
    console_hex(info_address);
    console_puts("\n");

    if (magic != 0x2badb002 || !info_address) {
        console_puts("[FAIL] invalid Multiboot handoff\n");
        goto halt;
    }
    const struct multiboot_info *info = (const void *)info_address;
    if (info->flags & 1) {
        console_puts("[memory] lower_KiB=");
        console_uint(info->mem_lower);
        console_puts(" upper_KiB=");
        console_uint(info->mem_upper);
        console_puts("\n");
    }
    if (bss_probe != 0 || data_probe != 0x11223344) {
        console_puts("[FAIL] ELF data/BSS initialization\n");
        goto halt;
    }
    console_puts("[OK] Multiboot handoff\n");
    console_puts("[OK] initialized data and zeroed BSS\n");
    console_puts("[READY] edit kernel/kernel.c and rebuild\n");

    if ((info->flags & (1u << 2)) && info->cmdline &&
        has_token((const char *)info->cmdline, "oslab.selftest=1")) {
        console_puts("[PASS] lab01 boot smoke test\n");
        /* Only the smoke-test QEMU command attaches isa-debug-exit. */
        outl(0xf4, 0x10);
    }
    runtime_start((info->flags & 4) && info->cmdline ? (const char *)info->cmdline : "");
halt:
    for (;;)
        __asm__ volatile ("cli; hlt");
}
