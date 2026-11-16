# 实验一：熟悉环境

## 要学什么

分清宿主 Arch 与目标内核，理解源代码、目标文件、ELF、加载地址、栈及 C 入口，学会调试一个真正的内核；

## 原始文件和步骤

```bash
make prepare LAB=01
cd work/lab01
make doctor
make
make lab-test LAB=01
make run
```

该副本提供完整启动基线，不挖空函数；修改 `kernel/kernel.c` 的启动名字，保持 `[OK]`、`[PASS]` 标记不变；

1. 用 `readelf -h -l -S build/kernel.elf` 检查 ELF32、入口、加载段及 `.text/.data/.bss/.user`；
2. 用 `nm -n`、`objdump -d -Mintel` 对应汇编和 C 函数；
3. 执行 `make debug-pipe`，在 `kernel_main` 查看参数、EIP、ESP、CR0；此时后续 GDT/IDT/分页基线还没有初始化；
4. 阅读 `boot/entry.S`，说明如何清零 BSS、建立栈、传递 EAX/EBX；
5. 运行 `make run` 后在串口 shell 输入 `syscall`、`ps`，区分代码由用户态还是内核态执行；

## 验收与思考

`make lab-test LAB=01` 通过，启动名字已经修改，并能解释文件偏移、虚拟/物理地址、NOLOAD、没有 libc 为何能运行；说明快速加载路径与下一个实验 BIOS 路径的区别；

## 提交与判断

保留修改的源码、实际命令、日志或调试记录，以及问题分析；自动测试只是固定场景，不能代替人工验收；根目录是参考基线，实验副本位于 `work/lab编号/`；不要把根目录演示通过当作自己已完成实验；
