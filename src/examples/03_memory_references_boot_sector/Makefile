BIN = ../bin

all: memory_references_boot_sector

clean:
	rm -rf $(BIN)/memory_references_boot_sector

memory_references_boot_sector: memory_references_boot_sector.asm
	nasm -f bin -o $(BIN)/memory_references_boot_sector memory_references_boot_sector.asm

run: memory_references_boot_sector
	qemu-system-x86_64 $(BIN)/memory_references_boot_sector
