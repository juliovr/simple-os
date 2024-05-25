# simple-os
Simple OS to understand the boot process and simple kernel of x86 architecture.

## Problems encountered and solutions
- The kernel doesn't load any global variables. The problem was that the boot loader does not load the whole kernel which fits, initially, in 9 sectors (i.e. 512 bytes * 9 = 4608 bytes = 0x1200 bytes). To solve this issue, execute `make dis` and look up how many bytes are in `os_image.dis` file, calculate how many sectors that number would be `(bytes - 512) / 512` (minus 512 because it excludes the boot sector) and paste that value in boot_sector.asm, line 35.
