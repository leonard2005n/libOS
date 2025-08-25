; function that prints a string located in bx register
print_string:
	push ax ; pushing the register that are used
	push bx

	mov ah, 0x0e ; int 10/ ah = 0 eh -> scrolling teletype BIOS routine

for_print_string:
	mov al, [bx] ; puting the string to print it
	cmp al, 0 ; check if it the last byte of the string
	je end_for_print_string
	int 0x10

	inc bx ; moving to the next byte
	jmp for_print_string
end_for_print_string:

	pop bx ; retriving the register used
	pop ax
	ret
