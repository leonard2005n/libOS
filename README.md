# libOS - A Learning Operating System

This is a beginner-friendly project designed for educational purposes - a simple operating system built from scratch to understand low-level system programming concepts.

## Current Features

- **Simple Bootloader**: Custom boot sector that loads the kernel
- **Basic Kernel**: Minimal kernel with video memory access
- **32-bit Protected Mode**: Transition from 16-bit real mode to 32-bit protected mode
- **GDT (Global Descriptor Table)**: Memory segmentation setup
- **Basic I/O Port Access**: Functions to read/write to hardware ports
- **Basic Print Functions**: Simple functions to print text to the screen

## Planned Features (TODO)

- [x] **Keyboard Driver**: Handle keyboard input and key mappings
- [x] **Video Driver**: Enhanced VGA text mode and graphics support
- [x] **PIC Initialization**: Programmable Interrupt Controller setup
- [ ] **Memory Management**: Paging, heap, and stack management
- [ ] **Interrupt Handling Framework**: Generalized interrupt/exception management
- [ ] **Timer Services**: System timer and scheduling
- [ ] **Simple File System**: Basic file operations and storage
- [ ] **Basic Shell/Command Interpreter**: Interactive command-line interface
- [ ] **Text Editor**: Simple text editing capabilities
- [ ] **Simple Compiler/Assembler**: Basic code compilation tools

## Project Structure

```
libOS/
├── Boot/           # Boot sector and boot utilities
│   ├── boot-sect.asm
│   ├── print/      # Print utilities
│   └── 32bit/      # 32-bit mode transition
├── Kernel/         # Kernel source code
│   ├── kernel.c
│   └── kernel_entry.asm
├── Drivers/        # Device drivers
│   ├── keyboard.c  # Keyboard driver (planned)
│   ├── video.c     # Video driver (planned)
│   └── ...         # Other drivers
├── Makefile        # Build configuration
└── README.md       # This file
```

## Prerequisites

Before building and running libOS, ensure you have the following tools installed:

- **NASM** (Netwide Assembler)
- **GCC** (GNU Compiler Collection)
- **LD** (GNU Linker)
- **QEMU** (Quick Emulator)

### Installation on Ubuntu/Debian:
```bash
sudo apt update
sudo apt install nasm gcc qemu-system-x86 build-essential
```

## How to Build and Run

1. **Clone the repository** (if applicable):
   ```bash
   git clone https://github.com/leonard2005n/libOS
   cd libOS
   ```

2. **Build the project**:
   ```bash
   make
   ```

3. **Run in QEMU**:
   ```bash
   make run
   ```

4. **Clean build files**:
   ```bash
   make clean
   ```

## Development Roadmap

### Phase 1: Core System ✅
- [x] Boot sector implementation
- [x] Kernel entry point
- [x] Basic video memory access
- [x] 32-bit protected mode

### Phase 2: Drivers ✅
- [x] Keyboard driver with interrupt handling
- [x] Enhanced video driver with text utilities
- [x] PIC (Programmable Interrupt Controller) initialization
- [x] Basic I/O port management

### Phase 3: System Services
- [ ] Memory management
- [ ] Interrupt handling framework
- [ ] Timer services

### Phase 4: User Interface
- [ ] Shell implementation
- [ ] Command parsing and execution
- [ ] Text editor

### Phase 5: File System
- [ ] Basic file system structure
- [ ] File operations (create, read, write, delete)
- [ ] Directory management

## Learning Resources

This project is perfect for understanding:
- x86 assembly language
- Boot process and BIOS interrupts
- Memory management and segmentation
- Device driver development
- Interrupt handling and I/O operations
- Kernel development basics
- Cross-compilation and linking

## Contributing

This is a learning project, but contributions and improvements are welcome! Feel free to submit issues or pull requests.