#include "keybord.h"
#include "low_level.h"
#include "screen.h"

key_event keyboard_buf[MAX_KEY_BUF];
unsigned char buf_position = 0; 
unsigned char cur_state;

// This function will initialize the keyboard variables
void keyboard_init(void)
{
	cur_state = NORMAL_STATE;
    // Unmask IRQ1 (keyboard interrupt) in the PIC
    unsigned char mask = inb(0x21);  // Read the current PIC mask
    mask &= ~0x02;                  // Clear bit 1 (IRQ1)
    outb(0x21, mask);  
}

// Function that reads form the ps2 controler the date form the keyboard
void keyboard_irq_handler(void)
{
	// Lookup table for Scan Code Set 1
	char scancode_set1[128][2] = {
		{0, 0}, {27, 27}, {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'},  // 0x00 - 0x07
		{'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'}, {'-', '_'}, {'=', '+'}, {8, 8}, {9, 9},    // 0x08 - 0x0F
		{'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'},  // 0x10 - 0x17
		{'o', 'O'}, {'p', 'P'}, {'[', '{'}, {']', '}'}, {13, 13}, {0, 0}, {'a', 'A'}, {'s', 'S'},  // 0x18 - 0x1F
		{'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'},  // 0x20 - 0x27
		{'\'', '"'}, {'`', '~'}, {0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'},  // 0x28 - 0x2F
		{'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'}, {0, 0}, {0, 0},    // 0x30 - 0x37
		{0, 0}, {' ', ' '}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},                        // 0x38 - 0x3F
		{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},                            // 0x40 - 0x47
		{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},                            // 0x48 - 0x4F
		{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},                            // 0x50 - 0x57
		{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}                             // 0x58 - 0x7F
	};

	// Get the scancode form the ps2 controller
	unsigned char scancode = inb(0x60);

	// This will set the state in which the scancode is
	if (scancode == 0xE0) {
		cur_state = PREFIX_STATE;
		outb(0x20, 0x20);
		__asm__ volatile ("sti");
		return;
	}

	// Store the next part of the scancode and the return to normal
	if (cur_state == PREFIX_STATE) {
		cur_state = NORMAL_STATE;
		outb(0x20, 0x20);
		__asm__ volatile ("sti");
		return;
	}

	bool key_released = scancode & 0x80;
    scancode &= 0x7F;

	// Putting the scancode in our scanodes history
	if (!key_released) {
		keyboard_buf[buf_position].code = scancode;
		keyboard_buf[buf_position].status_mask = 0;
        keyboard_buf[buf_position].shift_pressed = false;
        keyboard_buf[buf_position].alt_pressed = false;
        keyboard_buf[buf_position].ctrl_pressed = false;
        keyboard_buf[buf_position].caps_lock = false;

		// char ascii_char = get_char_from_scancode(keyboard_buf[buf_position]);
		char ascii_char = scancode_set1[scancode][0];

		const bool shifted = keyboard_buf[buf_position].status_mask & SHIFT_MASK || keyboard_buf[buf_position].caps_lock;
	
		if (shifted)
			ascii_char = scancode_set1[keyboard_buf[buf_position].code][1];
		else
			ascii_char = scancode_set1[keyboard_buf[buf_position].code][0];

		if (ascii_char) {
            print_char(ascii_char, -1, -1, WHITE_ON_BLACK);
        }
		
		buf_position++;
		buf_position = buf_position % MAX_KEY_BUF;
	}
    
	outb(0x20, 0x20);

	// Explicitly re-enable interrupts
    __asm__ volatile ("sti");
}

