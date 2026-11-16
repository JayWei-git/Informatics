/* Working teaching substrate; lab changes build on these small mechanisms. */
#include "runtime.h"
#define TASKS 12
#define PAGE 4096u
#define NSEMS 8
#define U 4u
#define SHARED 0x40000000u
#define PRIVATE (SHARED + PAGE)
#define READY 1
#define RUNNING 2
#define WAITING 3
#define DEAD 4
#define USER_CODE __attribute__((section(".user.text"), noinline))
#define USER_DATA __attribute__((section(".user.rodata")))

struct frame {
    u32 es, ds, edi, esi, ebp, oldesp, ebx, edx, ecx, eax;
    u32 vector, error, eip, cs, flags, useresp, ss;
};
struct task {
    struct frame *frame;
    u32 state, role, ticks, born, finished, waiting, switches;
    u32 *directory;
    u32 fd_active, fd_offset, fd_length;
    char fd_buffer[1024];
};
struct gate { u16 low, selector; u8 zero, flags; u16 high; } __attribute__((packed));
struct table_pointer { u16 size; u32 base; } __attribute__((packed));
struct semaphore { u32 count; int queue[TASKS]; u32 head, used; char name[16]; };
struct ring {
    u32 head, tail, count, consumed, sum, duplicate;
    u32 value[10], seen[32];
};

extern void load_gdt(const void *);
extern u32 isr_table[], __user_start[], __user_end[];
static unsigned long long gdt[6];
static u32 tss[26];
static struct gate idt[256];
static struct task task[TASKS];
static u8 stacks[TASKS][16384] __attribute__((aligned(PAGE)));
static u8 user_stacks[TASKS][PAGE] __attribute__((aligned(PAGE)));
static u32 directories[TASKS][1024] __attribute__((aligned(PAGE)));
static u32 low_tables[TASKS][1024] __attribute__((aligned(PAGE)));
static u32 high_tables[TASKS][1024] __attribute__((aligned(PAGE)));
static u32 identity[8][1024] __attribute__((aligned(PAGE)));
static u32 private_pages[TASKS][1024] __attribute__((aligned(PAGE)));
static u32 shared_page[1024] __attribute__((aligned(PAGE)));
static struct semaphore sem[NSEMS];
static u32 ticks, current, total = 1, quantum = 5, next_switch, mode;
static int trace, autoexit, terminal_probe;
static char name[24];
static u32 keyboard_code;

static void zero(void *pointer, u32 size)
{
    u8 *p = pointer;
    while (size--) *p++ = 0;
}
static int equal(const char *a, const char *b)
{
    while (*a && *a == *b) { ++a; ++b; }
    return *a == *b;
}
static int contains(const char *a, const char *b)
{
    for (; *a; ++a) {
        const char *x = a, *y = b;
        while (*x && *y && *x == *y) { ++x; ++y; }
        if (!*y) return 1;
    }
    return 0;
}
static void number(u32 n) { console_uint(n); }
static void event(u32 id, const char *state)
{
    if (!trace || !id) return;
    console_puts("TRACE\t"); number(id); console_puts("\t");
    console_puts(state); console_puts("\t"); number(ticks); console_puts("\n");
}
static void descriptor(u32 index, u32 base, u32 limit, u32 access)
{
    gdt[index] = (limit & 0xffffu) | ((unsigned long long)(base & 0xffffffu) << 16) |
        ((unsigned long long)access << 40) | ((unsigned long long)((limit >> 16) & 15) << 48) |
        ((unsigned long long)(base >> 24) << 56);
    if (index != 5) gdt[index] |= 0x00c0000000000000ull;
}
static void gate(u32 vector, u32 entry, u8 attributes)
{
    idt[vector] = (struct gate){entry & 0xffff, 8, 0, attributes, entry >> 16};
}
static void architecture(void)
{
    descriptor(1, 0, 0xfffff, 0x9a);
    descriptor(2, 0, 0xfffff, 0x92);
    descriptor(3, 0, 0xfffff, 0xfa);
    descriptor(4, 0, 0xfffff, 0xf2);
    tss[2] = 0x10;
    tss[25] = sizeof(tss) << 16;
    descriptor(5, (u32)tss, sizeof(tss) - 1, 0x89);
    struct table_pointer gp = {sizeof(gdt) - 1, (u32)gdt};
    load_gdt(&gp);
    __asm__ volatile ("ltr %0" : : "r"((u16)0x28));
    for (u32 i = 0; i < 32; ++i) gate(i, isr_table[i], 0x8e);
    gate(32, isr_table[32], 0x8e);
    gate(33, isr_table[33], 0x8e);
    gate(128, isr_table[34], 0xee);
    gate(129, isr_table[35], 0x8e);
    struct table_pointer ip = {sizeof(idt) - 1, (u32)idt};
    __asm__ volatile ("lidt %0" : : "m"(ip));
    outb(0x20, 0x11); outb(0xa0, 0x11);
    outb(0x21, 0x20); outb(0xa1, 0x28);
    outb(0x21, 4); outb(0xa1, 2);
    outb(0x21, 1); outb(0xa1, 1);
    outb(0x21, 0xfc); outb(0xa1, 0xff);
    u32 divisor = 1193182 / 100;
    outb(0x43, 0x36); outb(0x40, divisor & 255); outb(0x40, divisor >> 8);
}
static void paging(u32 id)
{
    for (u32 i = 0; i < 1024; ++i) low_tables[id][i] = i * PAGE | 3;
    for (u32 i = (u32)__user_start / PAGE; i < (u32)__user_end / PAGE; ++i)
        low_tables[id][i] = i * PAGE | 5;
    low_tables[id][(u32)user_stacks[id] / PAGE] |= U;
    directories[id][0] = (u32)low_tables[id] | 7;
    for (u32 i = 1; i < 8; ++i) directories[id][i] = (u32)identity[i] | 3;
    high_tables[id][0] = 0;
    high_tables[id][1] = (u32)private_pages[id] | 7;
    directories[id][SHARED >> 22] = (u32)high_tables[id] | 7;
    task[id].directory = directories[id];
}
static int user_range(u32 address, u32 length, int write)
{
    if (!length) return 1;
    if (address + length < address) return 0;
    u32 end = (address + length - 1) & ~(PAGE - 1);
    for (u32 p = address & ~(PAGE - 1);; p += PAGE) {
        u32 pde = task[current].directory[p >> 22];
        if ((pde & 7) != 7) return 0;
        u32 pte = ((u32 *)(pde & ~4095u))[(p >> 12) & 1023];
        if ((pte & 5) != 5 || (write && !(pte & 2))) return 0;
        if (p == end) return 1;
    }
}
static struct frame *schedule(struct frame *frame)
{
    /* TODO: implement this lab; consult its task sheet. */
    (void)frame; return frame;
}
static void sem_init(void)
{
    zero(sem, sizeof(sem));
}
static int sem_wait(u32 id)
{
    if (id >= NSEMS || !sem[id].name[0]) return -22;
    if (sem[id].count) { --sem[id].count; return 0; }
    struct semaphore *s = &sem[id];
    s->queue[(s->head + s->used) % TASKS] = (int)current;
    ++s->used;
    task[current].state = WAITING;
    task[current].waiting = id;
    event(current, "W");
    return 0;
}
static int sem_post(u32 id)
{
    if (id >= NSEMS || !sem[id].name[0]) return -22;
    struct semaphore *s = &sem[id];
    if (s->used) {
        u32 waiter = (u32)s->queue[s->head];
        s->head = (s->head + 1) % TASKS;
        --s->used;
        task[waiter].state = READY;
        event(waiter, "J");
    } else ++s->count;
    return 0;
}
static int sem_named(u32 address, u32 value, int unlink)
{
    char label[16]; u32 n;
    for (n = 0; n < sizeof(label); ++n) {
        if (address + n < address || !user_range(address + n, 1, 0)) return -14;
        label[n] = ((char *)address)[n];
        if (!label[n]) break;
    }
    if (!n || n == sizeof(label)) return -22;
    int available = -1;
    for (u32 i = 0; i < NSEMS; ++i) {
        if (!sem[i].name[0]) { if (available < 0) available = (int)i; continue; }
        if (equal(label, sem[i].name)) {
            if (!unlink) return (int)i + 1;
            if (sem[i].used) return -16;
            sem[i].name[0] = 0;
            return 0;
        }
    }
    if (unlink) return -2;
    if (available < 0) return -28;
    zero(&sem[available], sizeof(sem[available]));
    sem[available].count = value;
    for (u32 i = 0; i <= n; ++i) sem[available].name[i] = label[i];
    return available + 1;
}
static u32 proc_snapshot(char *buffer)
{
    char *p = buffer;
    const char *header = "pid state role ticks switches\n";
    while (*header) *p++ = *header++;
    for (u32 id = 0; id < total; ++id) {
        u32 values[] = {id, task[id].state, task[id].role, task[id].ticks, task[id].switches};
        for (u32 col = 0; col < 5; ++col) {
            char digits[10]; u32 n = 0, value = values[col];
            do { digits[n++] = (char)('0' + value % 10); value /= 10; } while (value);
            while (n) *p++ = digits[--n];
            *p++ = col == 4 ? '\n' : ' ';
        }
    }
    return (u32)(p - buffer);
}
static void show_proc(void)
{
    char snapshot[1024];
    u32 size = proc_snapshot(snapshot);
    snapshot[size] = 0;
    console_puts(snapshot);
}

static int sys_iam(u32 address)
{
    u32 length;
    for (length = 0; length <= 23; ++length) {
        if (address + length < address || !user_range(address + length, 1, 0)) return -14;
        if (!((char *)address)[length]) break;
    }
    if (length > 23) return -22;
    for (u32 i = 0; i <= length; ++i) name[i] = ((char *)address)[i];
    return (int)length;
}
static int sys_whoami(u32 address, u32 capacity)
{
    u32 length = 0; while (name[length]) ++length;
    if (capacity < length + 1) return -22;
    if (!user_range(address, length + 1, 1)) return -14;
    for (u32 i = 0; i <= length; ++i) ((char *)address)[i] = name[i];
    return (int)length;
}
static int shared_attach(u32 id)
{
    if (id != 1) return -22;
    high_tables[current][0] = (u32)shared_page | 7;
    __asm__ volatile ("invlpg (%0)" : : "r"(SHARED) : "memory");
    return (int)SHARED;
}
static int proc_open(u32 address)
{
    const char *path = "/proc/psinfo";
    for (u32 i = 0;; ++i) {
        if (address + i < address || !user_range(address + i, 1, 0)) return -14;
        if (((char *)address)[i] != path[i]) return -2;
        if (!path[i]) break;
    }
    task[current].fd_length = proc_snapshot(task[current].fd_buffer);
    task[current].fd_offset = 0;
    task[current].fd_active = 1;
    return 3;
}
static int proc_read(u32 fd, u32 address, u32 capacity)
{
    struct task *t = &task[current];
    if (fd != 3 || !t->fd_active) return -9;
    u32 remaining = t->fd_length - t->fd_offset;
    u32 count = capacity < remaining ? capacity : remaining;
    if (!user_range(address, count, 1)) return -14;
    for (u32 i = 0; i < count; ++i) ((char *)address)[i] = t->fd_buffer[t->fd_offset++];
    return (int)count;
}

/* All user code and constants occupy separate user-mapped pages. */
static const char hello[] USER_DATA = "[USER] hello through int 0x80\n";
static const char user_name[] USER_DATA = "ArchLearner";
static const char long_name[] USER_DATA = "123456789012345678901234";
static const char proc_path[] USER_DATA = "/proc/psinfo";
static const char empty_name[] USER_DATA = "empty";
static const char full_name[] USER_DATA = "full";
static const char mutex_name[] USER_DATA = "mutex";
static inline __attribute__((always_inline)) int syscall(u32 n, u32 a, u32 b, u32 c)
{
    int result;
    __asm__ volatile ("int $0x80" : "=a"(result) : "a"(n), "b"(a), "c"(b), "d"(c) : "memory", "cc");
    return result;
}
static USER_CODE __attribute__((noreturn)) void user_program(void)
{
    u32 role = (u32)syscall(3, 0, 0, 0);
    if (role == 10) {
        syscall(0, (u32)hello, sizeof(hello) - 1, 0);
        char buffer[24];
        int a = syscall(6, (u32)user_name, 0, 0);
        int b = syscall(7, (u32)buffer, sizeof(buffer), 0);
        int bad = syscall(0, 0x100000, 4, 0);
        int short_buffer = syscall(7, (u32)buffer, 2, 0);
        int too_long = syscall(6, (u32)long_name, 0, 0);
        int invalid = syscall(999, 0, 0, 0);
        int handle = syscall(16, (u32)mutex_name, 1, 0);
        int same = syscall(16, (u32)mutex_name, 99, 0);
        int removed = syscall(17, (u32)mutex_name, 0, 0);
        syscall(1, (a == 11 && b == 11 && bad == -14 && short_buffer == -22 && too_long == -22 && invalid == -38 && handle > 0 && handle == same && removed == 0) ? 0 : 1, 0, 0);
    } else if (role >= 20 && role <= 22) {
        int id = syscall(14, 7, PAGE, 0);
        if (id != 1 || (u32)syscall(15, (u32)id, 0, 0) != SHARED) syscall(1, 1, 0, 0);
        volatile struct ring *r = (volatile struct ring *)SHARED;
        u32 empty = (u32)syscall(16, (u32)empty_name, 10, 0);
        u32 full = (u32)syscall(16, (u32)full_name, 0, 0);
        u32 mutex = (u32)syscall(16, (u32)mutex_name, 1, 0);
        if (role == 20) {
            for (u32 value = 0; value < 1002; ++value) {
                syscall(4, empty, 0, 0); syscall(4, mutex, 0, 0);
                r->value[r->head] = value < 1000 ? value : 0xffffffffu;
                r->head = (r->head + 1) % 10; ++r->count;
                if (r->count > 10) ++r->duplicate;
                syscall(5, mutex, 0, 0); syscall(5, full, 0, 0);
            }
        } else {
            for (;;) {
                syscall(4, full, 0, 0); syscall(4, mutex, 0, 0);
                u32 value = r->value[r->tail];
                r->tail = (r->tail + 1) % 10; --r->count;
                if (value != 0xffffffffu) {
                    if (value >= 1000 || (r->seen[value / 32] & (1u << (value % 32)))) ++r->duplicate;
                    else r->seen[value / 32] |= 1u << (value % 32);
                    ++r->consumed; r->sum += value;
                }
                syscall(5, mutex, 0, 0); syscall(5, empty, 0, 0);
                if (value == 0xffffffffu) break;
            }
        }
        syscall(1, 0, 0, 0);
    } else if (role == 30 || role == 31) {
        volatile u32 *private_value = (volatile u32 *)PRIVATE;
        *private_value = role;
        for (u32 i = 0; i < 100; ++i) syscall(2, 0, 0, 0);
        syscall(1, *private_value == role ? 0 : 1, 0, 0);
    } else if (role == 70) {
        volatile u32 *value = (volatile u32 *)PRIVATE;
        *value = 0x12345678;
        while (*value) __asm__ volatile ("pause");
        syscall(1, 0, 0, 0);
    } else if (role == 50) {
        int fd = syscall(11, (u32)proc_path, 0, 0);
        char buffer[17]; int size, count = 0;
        if (fd != 3) syscall(1, 1, 0, 0);
        while ((size = syscall(12, (u32)fd, (u32)buffer, sizeof(buffer))) > 0) {
            syscall(0, (u32)buffer, (u32)size, 0); count += size;
        }
        int closed = syscall(13, (u32)fd, 0, 0);
        int bad = syscall(12, (u32)fd, (u32)buffer, sizeof(buffer));
        syscall(1, (size == 0 && count > 30 && closed == 0 && bad == -9) ? 0 : 1, 0, 0);
    } else if (role == 60) {
        /* An expected protection fault validates that Ring 3 cannot read the kernel. */
        u32 value = *(volatile u32 *)0x100000;
        (void)value;
        syscall(1, 1, 0, 0);
    } else {
        u32 begin = (u32)syscall(9, 0, 0, 0);
        volatile u32 value = 0;
        int slept = 0;
        while ((u32)syscall(9, 0, 0, 0) - begin < 25) {
            ++value;
            if (role == 41 && !slept && (u32)syscall(9, 0, 0, 0) - begin >= 10) {
                slept = 1; syscall(10, 15, 0, 0);
            }
        }
        syscall(1, value ? 0 : 1, 0, 0);
    }
    for (;;) __asm__ volatile ("ud2");
}

static u32 failures;
static void create(u32 role)
{
    if (total >= TASKS) return;
    u32 id = total++;
    struct task *t = &task[id];
    zero(t, sizeof(*t));
    paging(id);
    t->state = READY; t->role = role; t->born = ticks;
    t->frame = (struct frame *)((u32)&stacks[id][sizeof(stacks[id])] - sizeof(struct frame));
    zero(t->frame, sizeof(*t->frame));
    t->frame->ds = t->frame->es = t->frame->ss = 0x23;
    t->frame->cs = 0x1b; t->frame->flags = 0x202;
    t->frame->eip = (u32)user_program;
    t->frame->useresp = (u32)&user_stacks[id][PAGE] - 4;
    event(id, "N"); event(id, "J");
}
static void demo(const char *command)
{
    for (u32 i = 1; i < total; ++i)
        if (task[i].state != DEAD) { console_puts("[BUSY] wait for active tasks\n"); return; }
    total = 1; failures = 0; trace = contains(command, "trace");
    quantum = contains(command, "q=1") ? 1 : 5;
    mode = contains(command, "sem") || contains(command, "shared") ? 2 :
           contains(command, "private") ? 3 : contains(command, "trace") ? 4 :
           contains(command, "proc") ? 5 : contains(command, "fault") ? 6 :
           contains(command, "address") ? 7 : 1;
    zero(shared_page, sizeof(shared_page)); sem_init();
    if (mode == 1) create(10);
    if (mode == 2) { create(20); create(21); create(22); }
    if (mode == 3) { create(30); create(31); }
    if (mode == 4) { create(40); create(41); create(42); }
    if (mode == 5) create(50);
    if (mode == 6) create(60);
    if (mode == 7) create(70);
}
static void done_check(void)
{
    if (!mode) return;
    for (u32 i = 1; i < total; ++i) if (task[i].state != DEAD) return;
    if (mode == 2) {
        struct ring *r = (struct ring *)shared_page;
        console_puts("[PC] consumed="); number(r->consumed);
        console_puts(" sum="); number(r->sum);
        console_puts(" duplicate="); number(r->duplicate); console_puts("\n");
        if (r->consumed != 1000 || r->sum != 499500 || r->duplicate || r->count) ++failures;
    }
    console_puts(failures ? "[FAIL] runtime demo\n" : "[PASS] runtime demo\n");
    show_proc(); mode = 0;
    if (autoexit) outl(0xf4, failures ? 0x11 : 0x10);
}

struct frame *interrupt_dispatch(struct frame *f)
{
    if (f->vector < 32) {
        u32 cr2; __asm__ volatile ("mov %%cr2, %0" : "=r"(cr2));
        console_puts("[EXCEPTION] vector="); number(f->vector);
        console_puts(" eip="); console_hex(f->eip);
        console_puts(" cr2="); console_hex(cr2); console_puts("\n");
        if (current) {
            if (task[current].role != 60 || f->vector != 14 || cr2 != 0x100000 || !(f->error & 4)) ++failures;
            else console_puts("[OK] user/kernel page protection\n");
            task[current].state = DEAD; task[current].finished = ticks; event(current, "E");
            return schedule(f);
        }
        for (;;) __asm__ volatile ("cli; hlt");
    }
    if (f->vector == 32) {
        ++ticks; ++task[current].ticks;
        for (u32 i = 1; i < total; ++i) {
            if (task[i].state == WAITING && task[i].waiting == NSEMS && ticks >= task[i].finished) {
                task[i].state = READY; event(i, "J");
            }
        }
        outb(0x20, 0x20);
        if (ticks >= next_switch) return schedule(f);
    } else if (f->vector == 33) {
        keyboard_code = inb(0x60);
        if (keyboard_code == 0x58) console_filter_toggle();
        outb(0x20, 0x20);
    } else if (f->vector == 129) return schedule(f);
    else if (f->vector == 128) {
        u32 n = f->eax, a = f->ebx, b = f->ecx;
        f->eax = (u32)-38;
        if (n == 0) {
            if (b > 4096 || !user_range(a, b, 0)) f->eax = (u32)-14;
            else { for (u32 i = 0; i < b; ++i) { char c[2] = {((char *)a)[i], 0}; console_puts(c); } f->eax = b; }
        } else if (n == 1) {
            if (a) ++failures;
            task[current].state = DEAD; task[current].finished = ticks; event(current, "E");
            return schedule(f);
        } else if (n == 2) { f->eax = 0; return schedule(f); }
        else if (n == 3) f->eax = task[current].role;
        else if (n == 4) { f->eax = (u32)sem_wait(a - 1); if (task[current].state == WAITING) return schedule(f); }
        else if (n == 5) f->eax = (u32)sem_post(a - 1);
        else if (n == 6) f->eax = (u32)sys_iam(a);
        else if (n == 7) f->eax = (u32)sys_whoami(a, b);
        else if (n == 8) f->eax = ticks;
        else if (n == 9) f->eax = task[current].ticks;
        else if (n == 10) {
            if (a > 10000) f->eax = (u32)-22;
            else { task[current].state = WAITING; task[current].waiting = NSEMS;
                task[current].finished = ticks + a; f->eax = 0; event(current, "W"); return schedule(f); }
        }
        else if (n == 11) f->eax = (u32)proc_open(a);
        else if (n == 12) f->eax = (u32)proc_read(a, b, f->edx);
        else if (n == 13) {
            if (a != 3 || !task[current].fd_active) f->eax = (u32)-9;
            else { task[current].fd_active = 0; f->eax = 0; }
        } else if (n == 14) f->eax = a == 7 && b && b <= PAGE ? 1 : (u32)-22;
        else if (n == 15) f->eax = (u32)shared_attach(a);
        else if (n == 16) f->eax = (u32)sem_named(a, b, 0);
        else if (n == 17) f->eax = (u32)sem_named(a, 0, 1);
    }
    return f;
}

void runtime_start(const char *command)
{
    architecture();
    for (u32 table = 0; table < 8; ++table)
        for (u32 i = 0; i < 1024; ++i) identity[table][i] = (table * 1024 + i) * PAGE | 3;
    paging(0);
    task[0].state = RUNNING;
    tss[1] = (u32)&stacks[0][sizeof(stacks[0])];
    __asm__ volatile ("mov %0, %%cr3" : : "r"((u32)directories[0]) : "memory");
    u32 cr0; __asm__ volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80010000u;
    __asm__ volatile ("mov %0, %%cr0" : : "r"(cr0) : "memory");
    console_puts("[BASE] GDT IDT PIC PIT paging ready; timer=100Hz\n");
    autoexit = contains(command, "oslab.runtime-test=1");
    terminal_probe = contains(command, "demo=terminal");
    if (contains(command, "demo=") && !terminal_probe) demo(command);
    console_puts("[SHELL] help | syscall | trace | trace q=1 | sem | shared | private | ps | filter\n> ");
    __asm__ volatile ("sti");
    char line[64]; u32 length = 0, last_probe = 0;
    for (;;) {
        /* Avoid racing task creation with the timer scheduler. */
        __asm__ volatile ("cli");
        done_check();
        if (terminal_probe && ticks - last_probe >= 10) {
            last_probe = ticks;
            console_puts("AbC-123!\n");
        }
        if (inb(0x3f8 + 5) & 1) {
            char c = (char)inb(0x3f8);
            if (c == '\r' || c == '\n') {
                line[length] = 0; console_puts("\n");
                if (equal(line, "ps") || equal(line, "cat /proc/psinfo")) show_proc();
                else if (equal(line, "filter")) console_filter_toggle();
                else if (equal(line, "help")) console_puts("syscall trace sem shared private ps filter\n");
                else if (length) demo(line);
                length = 0; console_puts("> ");
            } else if ((c == 8 || c == 127) && length) --length;
            else if (c >= 32 && c <= 126 && length < sizeof(line) - 1) {
                line[length++] = c; char echo[2] = {c, 0}; console_puts(echo);
            }
        }
        __asm__ volatile ("sti; hlt");
    }
}
