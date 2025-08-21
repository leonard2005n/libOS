; 
[org 0x7c00] ; Tell the assembler where this code will be loaded

	mov byte [BOOT_DRIVE], dl

	mov bp, 0x8000 ; Setting the stack in a safe zone
	mov sp, bp
 
	mov bx, 0x9000  ; Load 2 sectors to 0x0000 (ES):0x9000 (BX)
	mov dh, 2		; from the boot disk.
	mov dl, byte [BOOT_DRIVE]
	call disk_load

	mov dx, [0x9000]
	call print_hex

	mov dx, [0x9000 + 512]
	call print_hex

	jmp $

%include "./print/print_hex.asm"
%include "./print/print_string.asm"
%include "disk_load.asm"

; Global variables
BOOT_DRIVE: db 0

boot_os_message:
	db 'Welcome to libOS!!! Booting OS...', 0

; Padding and magic BIOS number.
times 510-($-$$) db 0
dw 0xaa55

times 256 dw 0xdada
times 256 dw 0xface