BIN = ../../bin

all: os_image

clean:
	rm -rf \
		$(BIN)/kernel_boot_sector \
		$(BIN)/kernel_entry.o \
		$(BIN)/kernel.o \
		$(BIN)/kernel.bin \
		$(BIN)/os_image

kernel_boot_sector: kernel_boot_sector.asm
	nasm -f bin -o $(BIN)/kernel_boot_sector kernel_boot_sector.asm

kernel_entry.o: kernel_entry.asm
	nasm -f elf -o $(BIN)/kernel_entry.o kernel_entry.asm

kernel.o: kernel.c
	gcc -m32 -fno-pie -ffreestanding -c kernel.c -o $(BIN)/kernel.o

kernel.bin: kernel_entry.o kernel.o
	ld -m elf_i386 -o $(BIN)/kernel.bin -Ttext 0x1000 $(BIN)/kernel_entry.o $(BIN)/kernel.o --oformat=binary

os_image: kernel_boot_sector kernel.bin
	cat $(BIN)/kernel_boot_sector $(BIN)/kernel.bin > $(BIN)/os_image

run: all
	qemu-system-x86_64 $(BIN)/os_image
