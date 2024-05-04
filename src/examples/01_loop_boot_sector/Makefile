BIN = ../bin

all: loop_boot_sector

clean:
	rm -rf $(BIN)/loop_boot_sector

loop_boot_sector: loop_boot_sector.asm
	nasm -f bin -o $(BIN)/loop_boot_sector loop_boot_sector.asm

run: loop_boot_sector
	qemu-system-x86_64 $(BIN)/loop_boot_sector
