[bits 32]
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; prinst a null terminated string pointed by EBX
print_string_32bit:

	push edx ; storing on the stack the register used
	push eax
	push ebx
	
	mov edx, VIDEO_MEMORY ; preping the edx register to the
						  ; video memory

for_print_string_32bit:
	mov al, byte [ebx] ; storing the charater in al
	mov ah, WHITE_ON_BLACK ; put the atributes in ah

	cmp al, 0 ; check if the string has ended
	je end_for_print_string_32bit

	mov word[edx], ax ; stores the characther and the atributes in vid mem

	inc ebx ; moves to the next charachter that needs to be printed
	add edx, 2 ; jumps to the next character in vid mem

	jmp for_print_string_32bit ; jumping to the loop

end_for_print_string_32bit:

	pop ebx ; retriving the register used
	pop eax
	pop edx
	ret ; return from the function