BIN = bin
BOOT = src/boot
KERNEL = src/kernel

all: os_image

run: all
	qemu-system-x86_64 -d int -no-reboot -drive format=raw,file=$(BIN)/os_image

# Use to debug with gdb:
#     1. Run the command: gdb vmlinux
#     2. Inside gdb, execute: target remote localhost:1234
#     3. Use gdb as always
debug: all
	qemu-system-x86_64 -s -S -d int -no-reboot -drive format=raw,file=$(BIN)/os_image

dis: all
	objdump -D $(BIN)/kernel.o > $(BIN)/kernel.o.dump && \
	xxd $(BIN)/kernel.bin > $(BIN)/kernel.bin.dis && \
	xxd $(BIN)/os_image > $(BIN)/os_image.dis

clean:
	rm -rf \
		$(BIN)/boot_sector \
		$(BIN)/kernel_entry.o \
		$(BIN)/kernel.o \
		$(BIN)/kernel.bin \
		$(BIN)/os_image

# It has to cd in to the folder because the relative path of %include in nasm
# from where the nasm is called from, not where the file is located.
# Makefile cannot use pushd/popd, which would make it easier.
boot_sector: $(BOOT)/boot_sector.asm
	cd $(BOOT) && \
	nasm -f bin -o ../../$(BIN)/boot_sector boot_sector.asm && \
	cd $(CURDIR)

kernel_entry.o: $(BOOT)/kernel_entry.asm
	nasm -f elf -o $(BIN)/kernel_entry.o $(BOOT)/kernel_entry.asm

kernel.o: $(KERNEL)/kernel.c
	gcc -g -m32 -fno-pie -ffreestanding -c $(KERNEL)/kernel.c -o $(BIN)/kernel.o

# TODO: add IDT in a new asm file. Now is in kernel_entry.asm.

kernel.bin: kernel_entry.o kernel.o
	ld -m elf_i386 -Ttext=0x1000 --oformat binary -o $(BIN)/kernel.bin $(BIN)/kernel_entry.o $(BIN)/kernel.o

os_image: boot_sector kernel.bin
	cat $(BIN)/boot_sector $(BIN)/kernel.bin > $(BIN)/os_image

