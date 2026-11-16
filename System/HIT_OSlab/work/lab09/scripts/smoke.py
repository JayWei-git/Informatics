#!/usr/bin/env python3
"""Require both guest output and an explicit guest-triggered QEMU exit."""
import argparse
import shutil
import subprocess
import sys
from pathlib import Path

parser = argparse.ArgumentParser()
parser.add_argument("--qemu", default="qemu-system-i386")
parser.add_argument("kernel", type=Path)
args = parser.parse_args()
if not shutil.which(args.qemu):
    sys.exit("缺少 QEMU；执行 sudo pacman -Syu --needed qemu-system-x86 后重试 make test；")
command = [args.qemu, "-machine", "pc", "-accel", "tcg", "-cpu", "qemu32",
           "-m", "32M", "-smp", "1", "-kernel", str(args.kernel),
           "-append", "oslab.selftest=1", "-display", "none", "-serial", "stdio",
           "-monitor", "none", "-nic", "none", "-no-reboot",
           "-device", "isa-debug-exit,iobase=0xf4,iosize=0x04"]
try:
    result = subprocess.run(command, capture_output=True, text=True, timeout=15)
except subprocess.TimeoutExpired as error:
    log = (error.stdout or b"").decode(errors="replace")
    Path("build/boot-test.log").write_text(log)
    sys.exit("FAIL: 启动超过 15 秒；查看 build/boot-test.log；")
log = result.stdout + result.stderr
Path("build/boot-test.log").write_text(log)
required = ("[OK] Multiboot handoff", "[OK] initialized data and zeroed BSS",
            "[PASS] lab01 boot smoke test")
print(log, end="")
if result.returncode != 33 or not all(marker in log for marker in required):
    sys.exit(f"FAIL: guest exit={result.returncode}; expected 33 plus all boot markers")
print("PASS: 内核实际启动、自检输出和 QEMU 退出状态均符合预期；")
