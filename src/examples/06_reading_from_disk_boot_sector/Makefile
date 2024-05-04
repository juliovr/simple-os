BIN = ../bin

all: reading_from_disk_boot_sector

clean:
	rm -rf $(BIN)/reading_from_disk_boot_sector

reading_from_disk_boot_sector: reading_from_disk_boot_sector.asm
	nasm -f bin -o $(BIN)/reading_from_disk_boot_sector reading_from_disk_boot_sector.asm

run: all
	qemu-system-x86_64 $(BIN)/reading_from_disk_boot_sector
