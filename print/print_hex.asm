; transforms the nimbel in al into a char value coresponded
; to the hexa decimal
nibble_to_char:

	cmp al, 9 ; compares if it below 9
	jg not_digit

	add al, '0'

	ret
not_digit:

	sub al, 10 ; converts a to a char
	add al, 'A'
	ret

	; prints the value of DX as hex.
print_hex:
	push dx ; pushing the register that are used
	push bx
	push ax
	push cx

	xor cx, cx ; initialize the contor
	mov bx, HEX_OUT ; prints the string pointed to bx

	add bx, 5 ; move the pointer to the last nibble of the string

for_transform:

	mov al, dl ; getting the last nibble of the register
	and al, 0x0f

	call nibble_to_char ; converting the nibble to ascii
	mov [bx], byte al ; moving the nibble to the memory
	dec bx ; go down a characther
	shr dx, 4 ; moving the register in a nibble size

	inc cx
	cmp cx, 4
	je end_for_transofrm
	jmp for_transform

end_for_transofrm:

	mov bx, HEX_OUT ; prints the string pointed to bx
	call print_string

	pop cx  ; retriving the register used
	pop bx
	pop ax
	pop dx
	ret

; global variables
HEX_OUT: 
	db '0x0000' ,0