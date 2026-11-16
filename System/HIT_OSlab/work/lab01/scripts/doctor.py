#!/usr/bin/env python3
"""Report capabilities without installing packages or changing the host."""
import os
import shutil
import subprocess
import tempfile
from pathlib import Path

groups = {
    "编译和结构检查": ("gcc", "ld", "make", "python3"),
    "终端运行和自动启动测试": ("qemu-system-i386",),
    "GDB 调试": ("gdb",),
    "可选 GRUB ISO": ("grub-mkrescue", "grub-file", "xorriso"),
}
missing = False
for title, commands in groups.items():
    print(f"\n{title}：")
    for command in commands:
        location = shutil.which(command)
        if not title.startswith("可选"):
            missing |= location is None
        print(f"  {'OK' if location else 'MISSING':7} {command}: {location or '未安装'}")

if shutil.which("gcc") and shutil.which("ld"):
    with tempfile.TemporaryDirectory(prefix="oslab-doctor-") as directory:
        source = Path(directory) / "probe.c"
        obj = Path(directory) / "probe.o"
        source.write_text("void _start(void) { for (;;) __asm__ volatile (\"hlt\"); }\n")
        result = subprocess.run(
            ["gcc", "-m32", "-ffreestanding", "-fno-pie", "-fno-stack-protector",
             "-c", str(source), "-o", str(obj)], capture_output=True, text=True)
        if result.returncode == 0:
            result = subprocess.run(
                ["ld", "-m", "elf_i386", "-e", "_start", "-o",
                 str(Path(directory) / "probe.elf"), str(obj)],
                capture_output=True, text=True)
        print("\n32 位独立编译及链接：" + ("PASS" if result.returncode == 0 else "FAIL"))
        if result.returncode:
            missing = True
            print(result.stderr)

print("\n运行使用软件模拟 TCG，不要求 KVM、root 或 32 位 libc；")
if not os.environ.get("DISPLAY"):
    print("未发现 DISPLAY；终端模式可用，GTK 图形模式需在桌面会话执行；")
print("可选 ISO：sudo pacman -Syu --needed grub xorriso")
print("可选 GTK：sudo pacman -Syu --needed qemu-ui-gtk")
if missing:
    print("\nArch 基础依赖：sudo pacman -Syu --needed base-devel python qemu-system-x86 gdb")
    raise SystemExit(1)
print("\nPASS: 基础编译、终端运行和调试依赖齐全；")
