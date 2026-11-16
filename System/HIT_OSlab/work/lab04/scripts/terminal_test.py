#!/usr/bin/env python3
"""Inject actual PS/2 F12 events through QEMU's monitor, then inspect UART output."""
import argparse
import subprocess
import time
from pathlib import Path

p = argparse.ArgumentParser()
p.add_argument("--qemu", default="qemu-system-i386")
a = p.parse_args()
log = Path("build/terminal-test.log")
log.unlink(missing_ok=True)
command = [a.qemu, "-machine", "pc", "-accel", "tcg", "-m", "32M", "-kernel", "build/kernel.elf",
           "-append", "demo=terminal", "-display", "none", "-serial", f"file:{log}",
           "-monitor", "stdio", "-nic", "none", "-no-reboot"]
vm = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
stage = 0
try:
    deadline = time.monotonic() + 6
    while time.monotonic() < deadline and vm.poll() is None:
        text = log.read_text() if log.exists() else ""
        if stage == 0 and "AbC-123!" in text:
            vm.stdin.write(b"sendkey f12 20\n"); vm.stdin.flush(); stage = 1
        elif stage == 1 and "***-123!" in text:
            vm.stdin.write(b"sendkey f12 20\n"); vm.stdin.flush(); stage = 2
        elif stage == 2 and text.count("AbC-123!") >= 2:
            stage = 3; break
        time.sleep(0.02)
finally:
    if vm.poll() is None: vm.terminate()
    _, errors = vm.communicate(timeout=3)
if stage != 3:
    raise SystemExit(f"FAIL: F12 toggle stage={stage}; inspect {log}; {errors.decode(errors='replace')}")
print("PASS: actual PS/2 F12 interrupt toggles AbC-123! -> ***-123! -> AbC-123!")
