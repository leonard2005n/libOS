[org 0x7c00]

	mov ah, 0x0e


	mov bx, boot_os_message
	call print_string

	jmp $

%include "print_string.asm"

boot_os_message:
	db 'Welcome to libOS!!! Booting OS...', 0

; Padding and magic BIOS number.
times 510-($-$$) db 0

dw 0xaa55