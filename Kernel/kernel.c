#include "../Drivers/screen.h"
#include "../Drivers/keybord.h"
#include "../Drivers/low_level.h"
#include "../Drivers/interupts/interupt.h"


void main()
{
    clear_screen();
    print("Welcome to libOS!\n");
    print("Type something:\n");

	// Initialize idt
	idt_init();

    // Initialize the keyboard
    keyboard_init();
    __asm__ volatile ("sti");

	while (1) {

	}
}