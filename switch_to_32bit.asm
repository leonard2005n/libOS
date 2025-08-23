[bits 16]

switch_to_32bit:
	cli ; disable interupts
	
	lgdt [gdt_descriptor]

	mov eax, cr0 ; to make the swith to 32 bit pm we need
	or eax, 0x1  ; to set the first bit of cr0 to 1
	mov cr0, eax

	jmp CODE_SEG:init_32bit

[bits 32]
; initialize the registers and stack in p,
init_32bit:

	mov ax, DATA_SEG ; initialize old segments 
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x9000 ; initialize the stack
	mov esp, ebp

	call start_pm