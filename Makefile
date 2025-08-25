# Simple Makefile for libOS boot sector

ASM = nasm
ASMFLAGS = -f bin
QEMU = qemu-system-i386
CC = gcc
CFLAGS = -ffreestanding
LD = ld
LDFLAGS = -o Kernel/kernel.bin -Ttext 0x1000 --oformat binary

all: os-image

os-image: boot-sect.bin Kernel/kernel.bin
	cat boot-sect.bin Kernel/kernel.bin > os-image

boot-sect.bin: boot-sect.asm ./print/print_string.asm ./print/print_hex.asm ./32bit/disk_load.asm ./32bit/gdt.asm ./32bit/switch_to_32bit.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

Kernel/kernel.o: Kernel/kernel.c
	$(CC) $(CFLAGS) -c Kernel/kernel.c -o Kernel/kernel.o

Kernel/kernel.bin: Kernel/kernel.o
	$(LD) $(LDFLAGS) Kernel/kernel.o

run: os-image
	$(QEMU) -drive format=raw,file=os-image

clean:
	rm -f boot-sect.bin Kernel/kernel.o Kernel/kernel.bin os-image