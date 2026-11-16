SHELL := /bin/bash
.DEFAULT_GOAL := all
.DELETE_ON_ERROR:
CC := gcc
LD := ld
QEMU ?= qemu-system-i386
PYTHON ?= python3
BUILD := build
TARGET := $(BUILD)/kernel.elf
CFLAGS := -m32 -std=gnu11 -O2 -g3 -ffreestanding -fno-pie -fno-pic \
          -fno-stack-protector -fno-builtin -fno-asynchronous-unwind-tables \
          -fno-unwind-tables -fno-jump-tables -fno-tree-switch-conversion \
          -mno-sse -mno-sse2 -mno-mmx -msoft-float \
          -Wall -Wextra -Werror -MMD -MP -Iinclude
OBJECTS := $(BUILD)/boot.o $(BUILD)/kernel.o $(BUILD)/console.o $(BUILD)/runtime.o $(BUILD)/interrupts.o
$(OBJECTS): Makefile
QEMU_FLAGS := -machine pc -accel tcg -cpu qemu32 -m 32M -smp 1 \
              -no-reboot -no-shutdown -nic none

.PHONY: all check doctor run gui debug gdb debug-pipe iso run-iso test clean help bios bios-run bios-test bios-debug runtime-test stats host-test prepare lab-test
all: check

$(BUILD):
	mkdir -p $@

$(BUILD)/boot.o: boot/entry.S | $(BUILD)
	$(CC) -m32 -g3 -ffreestanding -fno-pie -c $< -o $@

$(BUILD)/%.o: kernel/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/interrupts.o: kernel/interrupts.S | $(BUILD)
	$(CC) -m32 -g -ffreestanding -fno-pie -c $< -o $@

$(TARGET): $(OBJECTS) linker.ld
	$(LD) -m elf_i386 --build-id=none -T linker.ld -Map $(BUILD)/kernel.map -o $@ $(OBJECTS)

check: $(TARGET)
	$(PYTHON) scripts/check_kernel.py $(TARGET)

doctor:
	$(PYTHON) scripts/doctor.py

DEMO ?=
APPEND := $(if $(DEMO),-append 'demo=$(DEMO)',)
run: check
	@command -v $(QEMU) >/dev/null || { echo '缺少 QEMU；请执行 sudo pacman -Syu --needed qemu-system-x86'; exit 1; }
	$(QEMU) $(QEMU_FLAGS) -kernel $(TARGET) $(APPEND) -display none -serial mon:stdio

gui: check
	$(QEMU) $(QEMU_FLAGS) -kernel $(TARGET) $(APPEND) -display gtk -serial stdio -monitor none

debug: check
	$(QEMU) $(QEMU_FLAGS) -kernel $(TARGET) $(APPEND) -display none -serial mon:stdio \
	  -S -gdb tcp:127.0.0.1:1234

gdb: $(TARGET)
	gdb -q -x scripts/kernel.gdb $(TARGET)

debug-pipe: check
	gdb -q $(TARGET) -ex 'set architecture i386' \
	  -ex 'target remote | $(QEMU) $(filter-out -no-shutdown,$(QEMU_FLAGS)) -kernel $(TARGET) $(if $(DEMO),-append "demo=$(DEMO)",) -display none -serial file:$(BUILD)/gdb-serial.log -monitor none -S -gdb stdio' \
	  -ex 'break kernel_main' -ex 'continue'

iso: check
	@command -v grub-mkrescue >/dev/null && command -v xorriso >/dev/null || { echo 'ISO 需要 grub 和 xorriso；请执行 sudo pacman -Syu --needed grub xorriso'; exit 1; }
	mkdir -p $(BUILD)/iso/boot/grub
	cp $(TARGET) $(BUILD)/iso/boot/kernel.elf
	cp boot/grub.cfg $(BUILD)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD)/oslab.iso $(BUILD)/iso

run-iso: iso
	$(QEMU) $(QEMU_FLAGS) -cdrom $(BUILD)/oslab.iso -boot d -display none -serial mon:stdio

test: check
	$(PYTHON) scripts/smoke.py --qemu $(QEMU) $(TARGET)

runtime-test: check
	$(PYTHON) scripts/runtime_test.py --qemu $(QEMU) $(if $(DEMO),--demo '$(DEMO)',)

stats: runtime-test
	$(PYTHON) tools/trace_stats.py build/runtime-trace.log --csv build/trace-q5.csv
	$(PYTHON) tools/trace_stats.py build/runtime-trace-q1.log --csv build/trace-q1.csv

$(BUILD)/host-pc: host/pc.c | $(BUILD)
	$(CC) -std=c11 -Wall -Wextra -Werror -O2 -g $< -pthread -o $@

host-test: $(BUILD)/host-pc
	timeout 15 $(BUILD)/host-pc file > $(BUILD)/host-file.log
	timeout 15 $(BUILD)/host-pc shared > $(BUILD)/host-shared.log
	@tail -n 1 $(BUILD)/host-file.log $(BUILD)/host-shared.log

LAB ?= 01
prepare:
	$(PYTHON) scripts/prepare_lab.py $(LAB)

lab-test:
	$(PYTHON) scripts/lab_test.py $(LAB) --qemu $(QEMU)

$(BUILD)/kernel.bin: $(TARGET)
	objcopy -O binary $< $@

$(BUILD)/layout.inc: $(BUILD)/kernel.bin scripts/bios_image.py
	$(PYTHON) scripts/bios_image.py layout

$(BUILD)/bootsect.o: bios/bootsect.S | $(BUILD)
	$(CC) -m32 -g -c $< -o $@

$(BUILD)/bootsect.bin: $(BUILD)/bootsect.o bios/bootsect.ld
	$(LD) -m elf_i386 -T bios/bootsect.ld -o $@ $<

$(BUILD)/setup.o: bios/setup.S $(BUILD)/layout.inc
	$(CC) -m32 -g -I$(BUILD) -c $< -o $@

$(BUILD)/setup.bin: $(BUILD)/setup.o bios/setup.ld
	$(LD) -m elf_i386 -T bios/setup.ld -o $@ $<

$(BUILD)/bios.img: $(BUILD)/bootsect.bin $(BUILD)/setup.bin $(BUILD)/kernel.bin
	$(PYTHON) scripts/bios_image.py image

bios: $(BUILD)/bios.img

bios-run: bios
	$(QEMU) $(QEMU_FLAGS) -drive file=$(BUILD)/bios.img,if=floppy,format=raw -boot a \
	  -display none -serial mon:stdio -debugcon file:$(BUILD)/bios-debug.log

bios-test: bios
	$(PYTHON) scripts/bios_smoke.py --qemu $(QEMU)

bios-debug: bios
	gdb -q $(TARGET) -ex 'set architecture i386' \
	  -ex 'target remote | $(QEMU) $(filter-out -no-shutdown,$(QEMU_FLAGS)) -drive file=$(BUILD)/bios.img,if=floppy,format=raw -boot a -display none -serial file:$(BUILD)/bios-serial.log -debugcon file:$(BUILD)/bios-debug.log -monitor none -S -gdb stdio' \
	  -ex 'hbreak *0x7c00' -ex 'continue'

clean:
	rm -rf -- $(BUILD)

help:
	@echo 'make            编译 ELF32 内核并检查 Multiboot 头'
	@echo 'make doctor     检查依赖，区分编译、运行、调试和 ISO'
	@echo 'make run        终端启动；Ctrl+a 后按 x 退出'
	@echo 'make gui        GTK 窗口启动，需要 QEMU GTK 模块'
	@echo 'make debug      暂停启动，等待本机 GDB 连接'
	@echo 'make gdb        在第二个终端连接 GDB'
	@echo 'make debug-pipe 单终端管道调试，无需监听端口'
	@echo 'make test       自动启动测试，检查内核实际输出及退出状态'
	@echo 'make runtime-test 验证用户态、系统调用、调度、同步、共享页和 proc'
	@echo 'make run DEMO=sem 运行指定内核演示：syscall/trace/sem/shared/private/proc/fault'
	@echo 'make stats      比较时间片 5 tick 和 1 tick 的进程轨迹'
	@echo 'make host-test  Arch 上真实多进程：文件缓冲区与共享映射'
	@echo 'make prepare LAB=03 生成不覆盖原文件的独立学习副本'
	@echo 'make lab-test LAB=03 验证指定实验，区别于启动自检'
	@echo 'make iso        可选：制作 GRUB 启动 ISO'
	@echo 'make bios-run   真正从 BIOS 引导扇区读取 setup 和内核'
	@echo 'make bios-test  验证 BIOS→setup→保护模式→C 内核'
	@echo 'make bios-debug 在 BIOS 加载的 0x7c00 处停住'
	@echo 'make run-iso    可选：从 ISO 启动'
	@echo 'make clean      仅清理本工程 build 目录'

-include $(OBJECTS:.o=.d)
