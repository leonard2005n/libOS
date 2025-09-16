#pragma once

#include <stdbool.h>
#define MAX_KEY_BUF 255
#define NORMAL_STATE 0
#define PREFIX_STATE 1
#define CTRL_MASK 1
#define ALT_MASK 2
#define SHIFT_MASK 3

typedef struct {
	unsigned char code;
	unsigned char status_mask;
	bool shift_pressed;
	bool alt_pressed;
	bool ctrl_pressed;
	bool caps_lock;
} key_event;

// This function will initialize the keyboard variables
void keyboard_init(void);

// Function that reads form the ps2 controler the date form the keyboard
void keyboard_irq_handler(void);
