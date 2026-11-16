#!/usr/bin/env python3
import argparse
import subprocess
import sys

p = argparse.ArgumentParser()
p.add_argument("lab", choices=[f"{i:02}" for i in range(1, 10)])
p.add_argument("--qemu", default="qemu-system-i386")
a = p.parse_args()
def run(*args):
    r = subprocess.run(list(args))
    if r.returncode: sys.exit(r.returncode)
def demo(name):
    run("make", "check")
    run("python3", "scripts/runtime_test.py", "--qemu", a.qemu, "--demo", name)

if a.lab == "01": run("make", "test", f"QEMU={a.qemu}")
elif a.lab == "02": run("make", "bios-test", f"QEMU={a.qemu}")
elif a.lab == "03": demo("syscall")
elif a.lab == "04":
    for name in ("trace", "trace q=1"):
        demo(name)
        log = "build/runtime-" + name.replace(" ", "-").replace("=", "") + ".log"
        run("python3", "tools/trace_stats.py", log)
elif a.lab == "05": demo("trace")
elif a.lab == "06":
    run("make", "host-test")
    demo("sem")
elif a.lab == "07":
    run("make", "host-test")
    for name in ("shared", "private", "fault"): demo(name)
elif a.lab == "08":
    run("make", "check")
    run("python3", "scripts/terminal_test.py", "--qemu", a.qemu)
elif a.lab == "09": demo("proc")
