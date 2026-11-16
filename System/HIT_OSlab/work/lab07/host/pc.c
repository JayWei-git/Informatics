#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define ITEMS 1000
#define CONSUMERS 2
struct buffer {
    unsigned head, tail, count, consumed, sum, bad;
    int values[10];
    unsigned char seen[ITEMS];
};
static int file;
static struct buffer *shared;
static sem_t *empty, *full, *mutex;

static void fail(const char *what) { perror(what); exit(1); }
static void wait_sem(sem_t *s)
{
    while (sem_wait(s) < 0) if (errno != EINTR) fail("sem_wait");
}
static void read_buffer(struct buffer *b)
{
    if (shared) { *b = *shared; return; }
    if (pread(file, b, sizeof(*b), 0) != (ssize_t)sizeof(*b)) fail("pread");
}
static void write_buffer(const struct buffer *b)
{
    if (shared) { *shared = *b; return; }
    if (pwrite(file, b, sizeof(*b), 0) != (ssize_t)sizeof(*b)) fail("pwrite");
}
static void producer(void)
{
    for (int value = 0; value < ITEMS + CONSUMERS; ++value) {
        wait_sem(empty); wait_sem(mutex);
        struct buffer b; read_buffer(&b);
        b.values[b.head] = value < ITEMS ? value : -1;
        b.head = (b.head + 1) % 10;
        if (++b.count > 10) ++b.bad;
        write_buffer(&b);
        if (sem_post(mutex) || sem_post(full)) fail("sem_post");
    }
}
static void consumer(void)
{
    for (;;) {
        wait_sem(full); wait_sem(mutex);
        struct buffer b; read_buffer(&b);
        int value = b.values[b.tail];
        b.tail = (b.tail + 1) % 10; --b.count;
        if (value != -1) {
            if (value < 0 || value >= ITEMS || b.seen[value]) ++b.bad;
            else b.seen[value] = 1;
            ++b.consumed; b.sum += (unsigned)value;
            printf("%ld: %d\n", (long)getpid(), value);
            fflush(stdout);
        }
        write_buffer(&b);
        if (sem_post(mutex) || sem_post(empty)) fail("sem_post");
        if (value == -1) return;
    }
}
int main(int argc, char **argv)
{
    int memory = argc > 1 && strcmp(argv[1], "shared") == 0;
    char path[] = "build/pc-buffer-XXXXXX";
    file = mkstemp(path);
    if (file < 0) fail("mkstemp");
    if (unlink(path) || ftruncate(file, sizeof(struct buffer))) fail("buffer file");
    if (memory) {
        shared = mmap(NULL, sizeof(*shared), PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
        if (shared == MAP_FAILED) fail("mmap");
    }
    struct buffer initial = {0}; write_buffer(&initial);
    sem_t **slots[] = {&empty, &full, &mutex};
    unsigned counts[] = {10, 0, 1};
    for (int i = 0; i < 3; ++i) {
        char label[64]; snprintf(label, sizeof(label), "/hit-oslab-%ld-%d", (long)getpid(), i);
        *slots[i] = sem_open(label, O_CREAT | O_EXCL, 0600, counts[i]);
        if (*slots[i] == SEM_FAILED) fail("sem_open");
        /* Children inherit live handles; immediate unlink prevents name leaks. */
        if (sem_unlink(label)) fail("sem_unlink");
    }
    pid_t children[CONSUMERS + 1];
    for (int i = 0; i <= CONSUMERS; ++i) {
        children[i] = fork();
        if (children[i] < 0) {
            /* Reap only children created by this invocation. */
            for (int j = 0; j < i; ++j) kill(children[j], SIGTERM);
            for (int j = 0; j < i; ++j) waitpid(children[j], NULL, 0);
            fail("fork");
        }
        if (!children[i]) { if (i == 0) producer(); else consumer(); _exit(0); }
    }
    int failed = 0;
    for (int i = 0; i <= CONSUMERS; ++i) {
        int status;
        pid_t child;
        do { child = waitpid(children[i], &status, 0); } while (child < 0 && errno == EINTR);
        if (child < 0 || !WIFEXITED(status) || WEXITSTATUS(status)) failed = 1;
    }
    struct buffer result; read_buffer(&result);
    failed |= result.consumed != ITEMS || result.sum != 499500 || result.bad || result.count;
    for (int i = 0; i < ITEMS; ++i) failed |= result.seen[i] != 1;
    printf("%s: %s consumed=%u sum=%u bad=%u\n", failed ? "FAIL" : "PASS",
           memory ? "shared mapping" : "file buffer", result.consumed, result.sum, result.bad);
    sem_close(empty); sem_close(full); sem_close(mutex);
    if (shared) munmap(shared, sizeof(*shared));
    close(file);
    return failed;
}
