#include "../low_level/low_level.h"
#include "../screen/screen.h"
#include "keyboard.h"

key_event keyboard_buf[MAX_KEY_BUF];
unsigned char buf_position = 0; 
unsigned char cur_state;
unsigned char current_modifiers;
bool caps_lock_status;


// This function will initialize the keyboard variables
void keyboard_init(void)
{
	cur_state = NORMAL_STATE;
	buf_position = 0;
	current_modifiers = 0;
	caps_lock_status = false;

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
	int scancode = inb(0x60);

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

		// If the key pressed and it is shift it will update the mask
		if (scancode == 0x2A || scancode == 0x36) {
        	current_modifiers |= 1 << SHIFT_MASK;
		}

		// if the key is pressed and it is ctrl it will update the mask
		if (scancode == 0x1D) {
        	current_modifiers |= 1 << CTRL_MASK;
		}

		// if the key is pressed and it is alt it will update the mask
		if (scancode == 0x38) {
			current_modifiers |= 1 << ALT_MASK;
		}

		// the caps lock is pressed
		if (scancode == 0x3A) {
			if(!caps_lock_status)
				caps_lock_status = true;
			else 
				caps_lock_status = false;
		}

		keyboard_buf[buf_position].caps_lock = caps_lock_status;
		keyboard_buf[buf_position].status_mask = current_modifiers;

		char ascii_char = scancode_set1[scancode][0];

		// Handle the shisfted part
		const bool shifted = (keyboard_buf[buf_position].status_mask & 
		(1 << SHIFT_MASK)) || keyboard_buf[buf_position].caps_lock;
	
		// Decides if it should get the shifted one
		if (shifted)
			ascii_char = scancode_set1[keyboard_buf[buf_position].code][1];
		else
			ascii_char = scancode_set1[keyboard_buf[buf_position].code][0];

		// Prints the character
		if (ascii_char) {
            print_char(ascii_char, -1, -1, WHITE_ON_BLACK);
        }
		
		// Increments the buffer
		buf_position++;
		buf_position = buf_position % MAX_KEY_BUF;
	} else {
		
		// if the shift key is released it will update the mask
		if (scancode == 0x2A || scancode == 0x36) {
			current_modifiers &= ~(1 << SHIFT_MASK);
		}

		// if the key is relessed and it is ctrl it will update the mask
		if (scancode == 0x1D) {
        	current_modifiers &= ~(1 << CTRL_MASK);
		}

		// if the key is released and it is alt it will update the mask
		if (scancode == 0x38) {
			current_modifiers &= ~(1 << ALT_MASK);
		}
	}
    
	outb(0x20, 0x20);

	// Explicitly re-enable interrupts
    __asm__ volatile ("sti");
}

