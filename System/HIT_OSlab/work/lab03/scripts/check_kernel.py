#!/usr/bin/env python3
"""Check the actual ELF layout against the Multiboot v1 loading contract."""
import struct
import sys
from pathlib import Path


def check(path):
    data = Path(path).read_bytes()
    if len(data) < 52 or data[:7] != b"\x7fELF\x01\x01\x01":
        raise ValueError("expected a little-endian ELF32 image")
    fields = struct.unpack_from("<HHIIIIIHHHHHH", data, 16)
    kind, machine, _, entry, phoff = fields[:5]
    phsize, phnum = fields[8:10]
    if kind != 2 or machine != 3 or phsize != 32 or not phnum:
        raise ValueError("expected an executable i386 ELF with program headers")
    found = None
    for offset in range(0, min(len(data), 8192) - 11, 4):
        magic, flags, checksum = struct.unpack_from("<III", data, offset)
        if magic == 0x1BADB002 and (magic + flags + checksum) & 0xFFFFFFFF == 0:
            if flags & ~3:
                raise ValueError("unexpected Multiboot requirements")
            found = offset
            break
    if found is None:
        raise ValueError("no aligned Multiboot header within the first 8192 bytes")
    executable_entry = False
    header_loaded = False
    bss_loaded = False
    for index in range(phnum):
        offset = phoff + index * phsize
        if offset + phsize > len(data):
            raise ValueError("truncated ELF program header table")
        typ, off, va, pa, filesz, memsz, flags, align = struct.unpack_from("<8I", data, offset)
        if typ == 3:
            raise ValueError("kernel must not require a userspace dynamic loader")
        if typ != 1:
            continue
        if off + filesz > len(data) or filesz > memsz:
            raise ValueError("invalid load segment bounds")
        if va != pa or pa < 0x100000:
            raise ValueError("expected identity-loaded segments at or above 1 MiB")
        if flags & 3 == 3:
            raise ValueError("unexpected writable executable load segment")
        if align > 1 and va % align != off % align:
            raise ValueError("invalid load segment alignment")
        executable_entry |= bool(flags & 1 and va <= entry < va + filesz)
        header_loaded |= off <= found and found + 12 <= off + filesz
        bss_loaded |= memsz > filesz
    if not executable_entry or not header_loaded or not bss_loaded:
        raise ValueError("entry, Multiboot header or BSS is missing from load segments")
    print(f"PASS: ELF32/i386, entry=0x{entry:08x}, Multiboot@{found}, load layout valid")


if __name__ == "__main__":
    try:
        check(sys.argv[1])
    except (IndexError, OSError, ValueError, struct.error) as error:
        sys.exit(f"FAIL: {error}")
