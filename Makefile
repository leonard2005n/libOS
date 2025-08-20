# Simple Makefile for libOS boot sector

ASM = nasm
ASMFLAGS = -f bin
QEMU = qemu-system-i386

all: boot-sect.bin

boot-sect.bin: boot-sect.asm print_string.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

run: boot-sect.bin
	$(QEMU) -drive format=raw,file=boot-sect.bin

clean:
	rm -f boot-sect.bin
