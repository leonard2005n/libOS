#include "screen.h"
#include "../low_level/low_level.h"

// Function that clears the screen
void clear_screen()
{
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            print_char(' ', col, row, WHITE_ON_BLACK);
        }
    }

	set_cursor(get_screen_offset(0, 0));
}

// Function that prints a string
void print(char *string)
{
	print_str(string, -1, -1);
}

// Function that prints a string on the display
void print_str(char *string, int col, int row)
{
	if (col >= 0 && row >= 0) {
		set_cursor(get_screen_offset(col, row));
	}

	int i = 0;
	while(string[i] != 0) {
		print_char(string[i], col, row, WHITE_ON_BLACK);
		i++;
	}
}

// A function that gets the offset from row anc col
int get_screen_offset(int col, int row)
{
	return ((row * 80 + col) * 2);
}

// Function that reads the cursor form the controlers
int get_cursor()
{
    int pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((int)inb(0x3D5)) << 8;
    return pos * 2;
}


// Function that sets the cursor on the controler
void set_cursor(int offset)
{
	int pos = offset / 2;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (int) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (int) ((pos >> 8) & 0xFF));
}

// Function that prints a characther
void print_char(char character, int col, int row, char attribute_byte)
{
	// Start of the video address
	unsigned char *vidmem = (char *) VIDEO_ADDRESS;

	if (!attribute_byte) {
		attribute_byte = WHITE_ON_BLACK;
	}

	int offset;

	// Getiing the offset of the display
	if (col >= 0 && row >= 0 && col < MAX_COLS && row < MAX_ROWS) {
		offset = get_screen_offset(col, row);
	} else {
		// If is not possible get the current cursor
		offset = get_cursor();
	}

	// If the character is new line go to the firs collum of the next row
	if (character == '\n') {
		int rows = offset / (2 * MAX_COLS);
		offset = get_screen_offset(79, rows);
	} else {
		vidmem[offset + 1] = attribute_byte;
		vidmem[offset] = character;
	}

	// Update the offset to the next character
	offset += 2;

	// Make the scrolling adjustemnts;
	offset = handle_scrolling(offset);

	// Update the cursor position
	set_cursor(offset);
}

// Function that scrolls the text on the screen
int handle_scrolling(int offset)
{
	// If the offset is in the scree reurn it
	if (offset < MAX_ROWS * MAX_COLS * 2)
		return offset;

	unsigned char *first_line = (char *) VIDEO_ADDRESS;
	unsigned char *second_line = first_line + get_screen_offset(0, 1);
	unsigned char *last_line = first_line + get_screen_offset(0, MAX_ROWS - 1);

	// Moves all the lines up a row
	mem_cpy(second_line, first_line, (MAX_ROWS - 1) * MAX_COLS * 2);

	// Makes the last line filled with
	for (int i = 0; i < MAX_COLS * 2; i++)
		last_line[i] = 0;

	// returns the offset of the firs element on the last line
	return get_screen_offset(0, MAX_ROWS - 1);
}
