# Simple Makefile for libOS boot sector

ASM = nasm
ASMFLAGS = -f bin
ASMFLAGS_ELF = -f elf32
QEMU = qemu-system-i386
CC = gcc
CFLAGS = -ffreestanding -m32 -fno-pie
LD = ld
LDFLAGS = -m elf_i386 -Ttext 0x1000 --oformat binary

# Recursively find all C and ASM source files in Drivers/
C_SOURCES := $(shell find Kernel -name '*.c') $(shell find Drivers -name '*.c')
ASM_SOURCES := $(shell find Drivers -name '*.asm')
HEADERS := $(shell find Kernel -name '*.h') $(shell find Drivers -name '*.h')
OBJ = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.asm=.o)

# Default build target
all: os-image

# Run QEMU to simulate booting of our code
run: all
	$(QEMU) -drive format=raw,file=os-image

# This is the actual disk image that the computer loads
# which is the combination of our compiled bootsector and kernel
os-image: Boot/boot-sect.bin Kernel/kernel.bin
	cat $^ > os-image

# This builds the binary of our kernel from two object files:
# - the kernel_entry, which jumps to main() in our kernel
# - the compiled C kernel
Kernel/kernel.bin: Kernel/kernel_entry.o $(OBJ)
	$(LD) $(LDFLAGS) -o $@ Kernel/kernel_entry.o $(OBJ)

# Generic rule for compiling C code to an object file
# For simplicity, we assume C files depend on all header files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble the kernel_entry
%.o: %.asm
	$(ASM) $(ASMFLAGS_ELF) $< -o $@

# Assemble the boot sector
%.bin: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# Build the boot sector binary
Boot/boot-sect.bin: Boot/boot-sect.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

# Compile the asm file
Drivers/interupts/isr.o: Drivers/interupts/isr.asm
	$(ASM) $(ASMFLAGS_ELF) -o $@ $<

# Clean up all generated files
clean:
	rm -f *.bin *.dis *.o os-image
	rm -f Kernel/*.o Kernel/kernel.bin
	rm -f Boot/*.bin
	find Drivers -name '*.o' -delete
	
