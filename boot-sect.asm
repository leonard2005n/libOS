;
; A boot sector that prints a string using our function.
;
[org 0x7c00] ; Tell the assembler where this code will be loaded

	mov bx, boot_os_message ; use bx as parameter for the function
	call print_string

	jmp $

%include "print_string.asm"

boot_os_message:
	db 'Welcome to libOS!!! Booting OS...', 0

; Padding and magic BIOS number.
times 510-($-$$) db 0

dw 0xaa55