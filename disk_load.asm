; load DH sectors to ES:BX from drive DL
disk_load:
	push dx ; storing the amount of sectors
			; that are loaded

	mov ah, 0x02
	mov al, dh
	mov ch, 0x00 ; select cylinder 0
	mov dh, 0x00 ; select head 0
	mov cl, 0x02 ; selects the second sector to read from

	int 0x13 ; Bios interrupt

	jc disk_error ; Jumps if error

	pop dx ; restore dx
	cmp dh, al ; compares the sectors read and the expected sectors
	jne disk_error ; Jumps if DH != AL the sectors
	ret

disk_error:
	mov bx, DISK_MSG_ERR ; displaying the error message
	call print_string
	jmp $

	pop dx

; Variables
DISK_MSG_ERR: db "DISK ERROR MESSAGE!!!", 0