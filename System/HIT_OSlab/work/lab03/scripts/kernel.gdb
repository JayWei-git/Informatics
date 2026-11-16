set pagination off
set architecture i386
set disassembly-flavor intel
target remote 127.0.0.1:1234
break kernel_main
continue
