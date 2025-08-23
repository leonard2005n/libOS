[org 0x7c00] ; Tell the assembler where this code will be loaded

	mov byte [BOOT_DRIVE], dl

	mov bp, 0x8000 ; Setting the stack in a safe zone
	mov sp, bp

	mov bx, mes_16bit
	call print_string
 
	call switch_to_32bit

	jmp $

%include "./print/print_hex.asm"
%include "./print/print_string.asm"
%include "disk_load.asm"
%include "gdt.asm"
%include "./print/print_string_32bit.asm"
%include "switch_to_32bit.asm"

; this is where are going to land after we enterd pm
start_pm:

	mov ebx, mes_32bit
	call print_string_32bit

	jmp $

; Global variables
BOOT_DRIVE: db 0

mes_16bit:
	db 'Start in 16 bit real mode', 0

mes_32bit:
	db 'Entered 32 bit protected mode',0

; Padding and magic BIOS number.
times 510-($-$$) db 0
dw 0xaa55

times 256 dw 0xdada
times 256 dw 0xface