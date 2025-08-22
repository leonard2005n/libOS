; GDT

gdt_start:

gdt_null: ; mendotory  null descriptor
	dw 0x0000
	dw 0x0000

gdt_code:

	; 1st flags: present = 1, privilege=00, type=1
	; type flags: executable=0, direction=0, readable=1, accessed=0
	; 2nd flags: granularity=1, size=1, long=0, reserved=0 
 
	dw 0xffff ; limit (0 - 15)
	dw 0x0000 ; base (0 - 15)
	db 0x00 ; base (16 - 23)
	db 10011010b ; 1st flags + type flags (bits 8 - 15)
	db 11001111b ; 2nd flags + seg limit (bits 16- 19)
	db 0x00 ; base (24 - 31)

gdt_data:

	; 1st flags: present = 1, privilege=00, type=1
	; type flags: executable=1, direction=0, readable=1, accessed=0
	; 2nd flags: granularity=1, size=1, long=0, reserved=0 
 
	dw 0xffff ; limit (0 - 15)
	dw 0x0000 ; base (0 - 15)
	db 0x00 ; base (16 - 23)
	db 10010010b ; 1st flags + type flags (bits 8 - 15)
	db 11001111b ; 2nd flags + seg limit (bits 16- 19)
	db 0x00 ; base (24 - 31)

gdt_end: ; reson we put the gdt here is to let the assembler to calculate
		 ; the size of the gdt

gdt_descriptor:

	dw gdt_end - gdt_start + 1 ; size of the our gdt
	dd gdt_start ; start address of the gdt

CODE_SEG equ gdt_code - gdt_start 
DATA_SEG equ gdt_data - gdt_start

