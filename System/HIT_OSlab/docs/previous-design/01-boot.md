# 实验一：启动与输出

## 起点

当前源码就是可运行基线；先保持原样完成 `make`、`make test`、`make run`，再修改代码；无需下载 Linux 0.11；

## 任务

1. 阅读 `boot/entry.S`，解释 Multiboot 头、魔数、校验和、栈建立、参数入栈和 `call` 的作用；
2. 阅读 `linker.ld`，找到 `.text`、`.data`、`.bss` 的加载地址，解释 `.bss` 为什么占内存却不需要同等大小的文件内容；
3. 在 `kernel/kernel.c` 修改启动名字，增加自己的学号或实验标识，不修改自动测试所需的 `[OK]` 和 `[PASS]` 标记；
4. 增加一个有初值的全局变量和一个无显式初值的全局变量，打印其地址和值，比较所属段；
5. 使用 GDB 在 `_start` 和 `kernel_main` 查看 EAX、EBX、ESP，解释启动信息如何进入 C 参数；
6. 可选：安装 GRUB 和 xorriso，使用 `make run-iso` 对比光盘启动与快速加载；

## 验收

- `make test` 通过，自己的启动文字可见；
- 能从链接映射或 ELF 段表说明三个段的位置；
- 能区分 Multiboot 头的魔数 `0x1badb002` 和入口 EAX 的魔数 `0x2badb002`；
- 能解释本实验为什么没有执行 BIOS `int 0x10`，却能输出文字；

## 建议记录

```bash
readelf -h -l -S build/kernel.elf
nm -n build/kernel.elf
objdump -d -Mintel build/kernel.elf
```

提交修改源码、启动日志、段布局数据和上述问题的分析；内核最后停机等待是预期行为，不要求实现 shell；
