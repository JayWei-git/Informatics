#!/usr/bin/env python3
import argparse
import subprocess
from pathlib import Path

available = ["syscall", "trace", "trace q=1", "sem", "shared", "private", "proc", "fault"]
p = argparse.ArgumentParser()
p.add_argument("--qemu", default="qemu-system-i386")
p.add_argument("--demo", choices=["all", *available], default="all")
a = p.parse_args()
choices = available
if a.demo != "all": choices = [a.demo]
for demo in choices:
    cmd = [a.qemu, "-machine", "pc", "-accel", "tcg", "-cpu", "qemu32", "-m", "32M",
           "-kernel", "build/kernel.elf", "-append", f"demo={demo} oslab.runtime-test=1",
           "-display", "none", "-serial", "stdio", "-monitor", "none", "-nic", "none",
           "-no-reboot", "-device", "isa-debug-exit,iobase=0xf4,iosize=0x04"]
    filename = Path("build") / ("runtime-" + demo.replace(" ", "-").replace("=", "") + ".log")
    try:
        r = subprocess.run(cmd, text=True, capture_output=True, timeout=12)
    except subprocess.TimeoutExpired as e:
        filename.write_bytes((e.stdout or b"") + (e.stderr or b""))
        raise SystemExit(f"FAIL: {demo} timeout; see {filename}")
    filename.write_text(r.stdout + r.stderr)
    if r.returncode != 33 or "[PASS] runtime demo" not in r.stdout or "[FAIL]" in r.stdout:
        raise SystemExit(f"FAIL: {demo}; see {filename}")
    if demo in ("sem", "shared") and "consumed=1000 sum=499500 duplicate=0" not in r.stdout:
        raise SystemExit(f"FAIL: {demo} consumption accounting")
    if demo == "proc" and "pid state role ticks switches" not in r.stdout:
        raise SystemExit("FAIL: proc content missing")
    if demo == "fault" and "[OK] user/kernel page protection" not in r.stdout:
        raise SystemExit("FAIL: expected protection fault missing")
    print(f"PASS: {demo}; log={filename}")
