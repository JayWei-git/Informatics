#!/usr/bin/env python3
"""Assemble a file-backed 1.44 MiB floppy image; never access host devices."""
import argparse
import struct
import subprocess
from pathlib import Path

parser = argparse.ArgumentParser()
parser.add_argument("phase", choices=["layout", "image"])
args = parser.parse_args()
build = Path("build")
if args.phase == "layout":
    kernel = (build / "kernel.bin").read_bytes()
    if not 0 < len(kernel) <= 65536:
        raise SystemExit("BIOS baseline supports a nonempty kernel of at most 64 KiB on disk")
    elf = (build / "kernel.elf").read_bytes()
    entry = struct.unpack_from("<I", elf, 24)[0]
    (build / "layout.inc").write_text(
        f"#define KERNEL_BYTES {len(kernel)}\n"
        f"#define KERNEL_SECTORS {(len(kernel) + 511) // 512}\n"
        f"#define KERNEL_ENTRY 0x{entry:x}\n")
else:
    boot = (build / "bootsect.bin").read_bytes()
    setup = (build / "setup.bin").read_bytes()
    kernel = (build / "kernel.bin").read_bytes()
    if len(boot) != 512 or boot[-2:] != b"\x55\xaa":
        raise SystemExit("invalid boot sector length or signature")
    if len(setup) > 2048:
        raise SystemExit("setup exceeds the four reserved sectors")
    image = boot + setup.ljust(2048, b"\0") + kernel
    if len(image) > 1474560:
        raise SystemExit("image exceeds a 1.44 MiB floppy")
    (build / "bios.img").write_bytes(image.ljust(1474560, b"\0"))
    print(f"PASS: BIOS image; boot=512, setup={len(setup)}, kernel={len(kernel)} bytes")
