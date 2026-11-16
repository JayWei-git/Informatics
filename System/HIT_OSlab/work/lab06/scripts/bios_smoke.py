#!/usr/bin/env python3
import argparse
import os
import selectors
import subprocess
import time
from pathlib import Path

p = argparse.ArgumentParser()
p.add_argument("--qemu", default="qemu-system-i386")
a = p.parse_args()
debug = Path("build/bios-debug.log")
debug.unlink(missing_ok=True)
command = [a.qemu, "-machine", "pc", "-accel", "tcg", "-m", "32M",
           "-drive", "file=build/bios.img,if=floppy,format=raw", "-boot", "a",
           "-display", "none", "-serial", "stdio", "-monitor", "none", "-nic", "none",
           "-debugcon", f"file:{debug}", "-no-reboot"]
output = bytearray()
vm = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
selector = selectors.DefaultSelector()
selector.register(vm.stdout, selectors.EVENT_READ)
passed = False
try:
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline and vm.poll() is None:
        for key, _ in selector.select(0.1):
            output.extend(os.read(key.fd, 4096))
        if b"[BASE] GDT IDT PIC PIT paging ready" in output and b"\n> " in output:
            passed = True
            break
finally:
    if vm.poll() is None: vm.terminate()
    rest, _ = vm.communicate(timeout=3)
    output.extend(rest)
    selector.close()
text = output.decode(errors="replace")
firmware = debug.read_text() if debug.exists() else ""
Path("build/bios-test.log").write_text(firmware + text)
expected = ("bootsect at 7c00", "Now we are in SETUP", "extended_KiB(hex)=", "kernel sectors loaded")
if not passed or not all(x in firmware for x in expected) or "[FAIL]" in text + firmware:
    raise SystemExit("FAIL: BIOS chain; inspect build/bios-test.log")
print(firmware + text)
print("PASS: real BIOS disk boot, setup, memory query, protected mode, C kernel and runtime initialization")
