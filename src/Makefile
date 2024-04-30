all: 

clean:
	rm -rf \
		../bin/loop_boot_sector \
		../bin/print_boot_sector

loop_boot_sector: loop_boot_sector.asm
	nasm -f bin -o ../bin/loop_boot_sector loop_boot_sector.asm
	qemu-system-x86_64 ../bin/loop_boot_sector

print_boot_sector: print_boot_sector.asm
	nasm -f bin -o ../bin/print_boot_sector print_boot_sector.asm
	qemu-system-x86_64 ../bin/print_boot_sector
