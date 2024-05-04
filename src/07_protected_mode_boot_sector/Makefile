BIN = ../../bin

all: protected_mode_boot_sector

clean:
	rm -rf $(BIN)/protected_mode_boot_sector

protected_mode_boot_sector: protected_mode_boot_sector.asm
	nasm -f bin -o $(BIN)/protected_mode_boot_sector protected_mode_boot_sector.asm

run: all
	qemu-system-x86_64 $(BIN)/protected_mode_boot_sector
