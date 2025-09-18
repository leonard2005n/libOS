#include "../Drivers/PIC/PIC.h"
#include "../Drivers/screen/screen.h"
#include "../Drivers/keyboard/keyboard.h"
#include "../Drivers/interupts/interupt.h"
#include "../Drivers/low_level/low_level.h"



void main()
{
    clear_screen();
    print("Welcome to libOS!\n");
    print("Type something:\n");

	// Initialize pic
	init_pic();

	// Initialize idt
	idt_init();

    // Initialize the keyboard
    keyboard_init();
    __asm__ volatile ("sti");

	while (1) {

	}
}