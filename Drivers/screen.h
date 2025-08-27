#pragma once

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

// Screen device I/O ports
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d6

// Function that clears the screen
void clear_screen();

// Function that prints a string
void print(char *string);

// Function that prints a string on the display
void print_str(char *string, int col, int row);

// A function that gets the offset from row anc col
int get_screen_offset(int col, int row);

// Function that reads the cursor form the controlers
int get_cursor();

// Function that sets the cursor on the controler
void set_cursor(int offset);

// Function that prints a characther
void print_char(char character, int col, int row, char attribute_byte);

// Function that scrolls the text on the screen
int handle_scrolling(int offset);
