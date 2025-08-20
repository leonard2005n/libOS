; function that prints a string located in bx register
print_string:
	push ax
	push bx

	mov ah, 0x0e

for_print_string:
	mov al, [bx]
	cmp al, 0
	je end_for_print_string
	int 0x10

	inc bx
	jmp for_print_string
end_for_print_string:

	pop bx
	pop ax
	ret