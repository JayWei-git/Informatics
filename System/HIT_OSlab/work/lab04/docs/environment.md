# 环境与调试

## 运行边界

宿主为 Arch Linux x86_64，目标为 QEMU 软件模拟的单核 32 位 x86 PC；不使用真实磁盘或本机引导配置；GCC -m32 只生成独立目标代码，ld -m elf_i386 链接，不需要 32 位 libc 或旧版编译器；

所有实验命令在当前工作副本目录执行；根目录是可运行参考，`work/lab03` 等是学习副本；构建输出都在各自的 build 目录；不自动 git commit、push 或重置用户文件；

## 两条启动路径

```text
BIOS 实验：固件 → 0x7c00 → bootsect 搬移 → int13 加载 setup
         → setup 查询硬件和读内核 → A20/GDT/PE → 内核入口 → C

内核快速迭代：QEMU Multiboot 加载器 → 内核入口 → C → 参考运行时
```

`make bios-run` 走第一条，`make run` 走第二条；两者最终运行同一个内核；快速加载器已经进入保护模式，但自己的 BIOS setup 会亲自完成转换；到 C 入口时分页尚未打开，`runtime_start` 才建立自己的 GDT、IDT、PIC/PIT、页表，再开放中断；

BIOS 镜像是 1.44 MiB 普通文件：第一个扇区为 bootsect，随后四个扇区预留给 setup，内核从第六个扇区开始；布局常量由构建工具从实际内核长度和 ELF 入口生成，不需要手改扇区数；当前限定内核磁盘内容不超过 64 KiB；BSS 不占这些磁盘字节，由汇编入口显式清零；

## 常用调试

```bash
make debug-pipe DEMO=private
```

单终端管道方式实际可用，不监听 TCP；GDB 在 kernel_main 停住；串口写到 build/gdb-serial.log；随后可以输入：

```gdb
info registers eip esp cr0 cr3
p/x magic
p/x info_address
break user_program
continue
info registers cs ss esp cr3
p current
p/x task[current].directory
x/8wx task[current].directory
```

用户模式执行与内核模式中断处理可以分别在 user_program、interrupt_dispatch、schedule 设断点；`kill` 结束 QEMU，再 `quit` 退出 GDB；

如果需要双终端，在终端一执行 `make debug DEMO=private`，终端二执行 `make gdb`；默认只监听 127.0.0.1:1234；当前 Agent 沙箱无法连接本机 TCP，不影响你的桌面终端，管道方式已验证；

BIOS 调试执行 `make bios-debug`，使用硬件断点停在 0x7c00，之后可在 0x90200 设断点跟踪 setup；实模式指令可通过 `objdump -dr -mi8086 build/bootsect.o` 辅助阅读，不能直接把默认 32 位反汇编结果当作 16 位指令；

优化级别为 -O2，部分局部变量会优化掉；需要时改为 -Og 并重新构建；关闭跳转表优化，避免同一 ELF 中的用户代码意外引用内核只读段的编译器跳转表；用户代码位于独立的 .user 页，权限为用户可读、不可写，内核页保持 supervisor；

## 系统调用 ABI

用户代码通过 int 0x80，EAX 为编号、EBX/ECX/EDX 为参数，EAX 返回结果；负数直接表示错误，没有宿主 libc 的 errno；

| 号 | 基线接口 |
| --- | --- |
| 0 | write(pointer,length)，仅控制台，单次最多4096字节 |
| 1 | exit(status) |
| 2 | yield() |
| 3 | get_role()，测试角色号，不是 POSIX getpid |
| 4、5 | sem_wait(handle)、sem_post(handle) |
| 6、7 | iam(pointer)、whoami(pointer,capacity) |
| 8、9 | 系统 tick、当前任务 CPU tick |
| 10 | sleep(ticks)，用于模拟等待型工作负载 |
| 11～13 | open(path)、read(fd,buffer,capacity)、close(fd)，只读 proc |
| 14、15 | shmget(key,size)、shmat(id)，当前只有 key=7 的一页 |
| 16、17 | sem_open(name,initial)、sem_unlink(name) |

返回值 -14 为坏地址、-22 为无效参数、-9 为无效描述符、-38 为未实现/未知调用；这些编号与常见 Linux 错误码一致，但本实验不是 Linux ABI 兼容层；

## 文件、日志与测试

- `build/boot-test.log`：快速启动自检；
- `build/bios-test.log`、`build/bios-debug.log`：实际 BIOS 引导链和调试端口输出；
- `build/runtime-*.log`：用户态、调度、同步、共享页、proc 和异常案例；
- `build/trace-q*.csv`：调度统计；
- `build/host-file.log`、`build/host-shared.log`：Arch 上真实多进程输出；
- `build/terminal-test.log`：PS/2 F12 注入测试；

make test 只验启动；make lab-test LAB=编号 才是该实验固定验收；03～09 的初始学习副本应编译成功但目标验收失败；自动测试结果不代替任务书中的手动页表追踪、新参数读取、新 proc 节点等作业要求；

## 可选图形与 ISO

`sudo pacman -Syu --needed qemu-ui-gtk` 后可执行 make gui；可选 GRUB 对照需要 `grub xorriso`，执行 make iso、make run-iso；不运行 grub-install、不修改真实分区；这些可选路径不是当前实验的前置条件；
