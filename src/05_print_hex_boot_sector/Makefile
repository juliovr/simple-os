BIN = ../../bin

all: print_hex_boot_sector

clean:
	rm -rf $(BIN)/print_hex_boot_sector

print_hex_boot_sector: print_hex_boot_sector.asm
	nasm -f bin -o $(BIN)/print_hex_boot_sector print_hex_boot_sector.asm

run: print_hex_boot_sector
	qemu-system-x86_64 $(BIN)/print_hex_boot_sector
