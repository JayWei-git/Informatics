# HIT OS Lab：Arch 原生版

这是一套参考哈工大操作系统实验学习目标重新设计的实验；保留 BIOS 引导、系统调用、进程轨迹、内核栈切换、同步互斥、地址映射、共享内存、终端和 proc 的核心学习内容，使用 Arch 当前 GCC、GNU 汇编器、QEMU、GDB；不依赖旧 Ubuntu、GCC 3.4 或 Linux 0.11 工具链；

根目录提供可运行的参考基线；`make prepare LAB=编号` 生成独立的学习副本，将对应函数留为 TODO，其他基础代码保持可用；你是在已有小内核上修改指定机制，不必从空目录造完整操作系统；

## 开始学习

基础依赖已经具备时直接执行；新机器安装使用 `sudo pacman -Syu --needed base-devel python qemu-system-x86 gdb`；无需 32 位 libc，也不需要 KVM；

```bash
cd /home/jaywei/Odyssey/Informatics/System/HIT_OSlab
make doctor
make test
make bios-test
make run
```

终端运行按 Ctrl+a 后再按 x 退出；参考系统的串口 shell 支持 `help`、`syscall`、`trace`、`trace q=1`、`sem`、`shared`、`private`、`proc`、`fault`、`ps`、`cat /proc/psinfo` 和 `filter`；

从第一个实验开始：阅读 [实验一](labs/01-environment.md)，执行 `make prepare LAB=01`；学习副本位于 `work/lab01/`；每个副本都可以独立编译、运行和调试；生成器拒绝覆盖已有副本；

例如做系统调用实验：

```bash
make prepare LAB=03
cd work/lab03
make
make lab-test LAB=03
```

此时实验测试失败是预期的，因为 `sys_iam`、`sys_whoami` 已留为 TODO；按 [实验三](labs/03-syscalls.md) 补齐后再验证；`make test` 只证明能启动，不能代替 `make lab-test LAB=03`；

## 九个实验

| 编号 | 学习目标及任务书 | 主要修改点 | 验证 |
| --- | --- | --- | --- |
| 01 | [熟悉环境](labs/01-environment.md) | 编译、ELF、GDB、启动路径 | `make lab-test LAB=01` |
| 02 | [操作系统引导](labs/02-boot.md) | `bios/bootsect.S`、`bios/setup.S` | `make lab-test LAB=02` |
| 03 | [系统调用](labs/03-syscalls.md) | `sys_iam`、`sys_whoami` | `make lab-test LAB=03` |
| 04 | [进程轨迹与统计](labs/04-trace.md) | `event`、工作负载、统计程序 | `make lab-test LAB=04` |
| 05 | [内核栈切换](labs/05-switch.md) | `schedule`、中断现场、CR3、TSS.ESP0 | `make lab-test LAB=05` |
| 06 | [信号量](labs/06-semaphores.md) | `sem_wait`、`sem_post`、命名接口 | `make lab-test LAB=06` |
| 07 | [地址映射与共享](labs/07-address-sharing.md) | `shared_attach`、页表、用户访问检查 | `make lab-test LAB=07` |
| 08 | [终端设备控制](labs/08-terminal.md) | F12 中断、`console_filter_toggle` | `make lab-test LAB=08` |
| 09 | [proc 文件接口](labs/09-proc.md) | `proc_snapshot`、open/read/close | `make lab-test LAB=09` |

和原课程的逐项对应、保留内容及简化边界见 [设计对照](docs/course-alignment.md)；实验 01、02 提供能运行的原始基线，再做定制和观测任务；实验 03～09 对应学习副本会留出目标函数；

## 常用命令

| 命令 | 用途 |
| --- | --- |
| `make` | 生成 ELF32 内核并检查加载布局 |
| `make run` | 快速进入参考内核及串口 shell |
| `make run DEMO=sem` | 启动指定演示，可选 syscall、trace、sem、shared、private、proc、fault |
| `make bios-run` | 真正经过 BIOS、引导扇区、setup，再进入相同内核 |
| `make bios-test` | 自动验证完整 BIOS 启动链 |
| `make bios-debug` | 单终端 GDB，在 BIOS 加载的物理地址 0x7c00 停住 |
| `make debug-pipe DEMO=private` | 单终端调试指定演示，在 kernel_main 停住 |
| `make debug` 与 `make gdb` | 双终端 TCP 调试，限本机 127.0.0.1:1234 |
| `make test` | 启动协议、数据初始化和 BSS 自检 |
| `make runtime-test` | 用户态、系统调用、调度、同步、共享页、隔离和 proc 基线测试 |
| `make stats` | 生成时间片 5 tick 和 1 tick 的调度统计、CSV |
| `make host-test` | Arch 上真实进程的文件缓冲区、共享映射实验 |
| `make lab-test LAB=08` | 注入真实 F12 按键，验证终端转换与恢复 |
| `make clean` | 仅删除当前工作副本的 build 目录 |

`make run` 用 QEMU 的 Multiboot 快速加载路径，方便后续内核实验；学习 BIOS 启动必须使用 `make bios-run`，不能用快速加载路径代替；`make iso`、`make run-iso` 是可选 GRUB 对照，不属于基础依赖；

## 目录

```text
bios/               真实 BIOS 引导扇区、setup、保护模式跳转
boot/               快速加载入口及可选 GRUB 配置
kernel/kernel.c     入口自检，进入参考运行时
kernel/runtime.c    GDT/IDT、页表、用户进程、调度、信号量和 proc
kernel/interrupts.S 中断入口、内核栈现场恢复、iret
kernel/console.c    VGA、串口及字母过滤
host/pc.c           Arch 多进程生产者—消费者，文件与共享映射双后端
scripts/            构建检查、工作副本生成、QEMU 自动验证、GDB
labs/               当前九个实验的要求、步骤、验收和报告问题
tools/              轨迹统计工具
work/               按需生成的独立学习副本，不覆盖已有进度
build/              当前工程的构建结果和测试日志，Git 忽略
```

参考基线当前采用固定任务槽、固定页面和一个只读 proc 节点，重点在实验机制；不提供完整 POSIX、ELF 用户程序加载器、fork、磁盘文件系统或通用内存分配器；用户程序已编译进 `.user` 段，工作副本可以直接修改；这些边界和原实验的差异已单列说明，不把演示通过称作完整 Linux 内核；

## 资料与验证

- [环境和调试](docs/environment.md)；
- [原课程与新设计的对应](docs/course-alignment.md)；
- [本机验收](docs/validation.md)；
- [参考课程实验记录](https://github.com/TonyWriting/Linux0.11)；
- [Multiboot v1 规范](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)；
- [QEMU 官方说明](https://www.qemu.org/docs/master/system/invocation.html)；

这是私人学习工程，说明只保存在本目录，不记录到主目录的系统维护 README；未自动提交、推送或备份；
