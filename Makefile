BIN = bin
BOOT = src/boot
KERNEL = src/kernel

all: os_image

run: all
	qemu-system-x86_64 $(BIN)/os_image

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
	gcc -m32 -fno-pie -ffreestanding -c $(KERNEL)/kernel.c -o $(BIN)/kernel.o

kernel.bin: kernel_entry.o kernel.o
	ld -m elf_i386 -Ttext 0x1000 --oformat binary -o $(BIN)/kernel.bin $(BIN)/kernel_entry.o $(BIN)/kernel.o

os_image: boot_sector kernel.bin
	cat $(BIN)/boot_sector $(BIN)/kernel.bin > $(BIN)/os_image

