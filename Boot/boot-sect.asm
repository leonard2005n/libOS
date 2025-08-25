[org 0x7c00] ; Tell the assembler where this code will be loaded
KERNEL_OFFSET equ 0x1000

	mov byte [BOOT_DRIVE], dl

	mov bp, 0x9000 ; Setting the stack in a safe zone
	mov sp, bp

	mov bx, mes_16bit ; printing the message that we are in 16 bits
	call print_string

	call load_kernel ; load kernel
 
	call switch_to_32bit ; switching to 32 bits

	jmp $

%include "./Boot/print/print_hex.asm"
%include "./Boot/print/print_string.asm"
%include "./Boot/32bit/disk_load.asm"
%include "./Boot/32bit/gdt.asm"
%include "./Boot/print/print_string_32bit.asm"
%include "./Boot/32bit/switch_to_32bit.asm"

[bits 16]

load_kernel:

	mov bx, mes_load_kernel ; printing the message that we are loading the kernel
	call print_string

	mov bx, KERNEL_OFFSET ; loading the offset of the kernel
	mov dh, 9 ; reading 9 sectors to load into memory
	mov dl, byte [BOOT_DRIVE] ; puting the boot device form wher to read
	call disk_load 

	ret

[bits 32]

; this is where are going to land after we enterd pm
start_pm:

	mov ebx, mes_32bit ; printing the message 
	call print_string_32bit

	call KERNEL_OFFSET ; calling the fucnton from the kerne;

	jmp $

; Global variables
BOOT_DRIVE: db 0

mes_16bit:
	db 'Start in 16 bit real mode   ', 0

mes_32bit:
	db 'Entered 32 bit protected mode',0

mes_load_kernel:
	db 'Loading Kernel', 0

; Padding and magic BIOS number.
times 510-($-$$) db 0
dw 0xaa55