#include "low_level.h"

// A function that reads a word from a specified port
unsigned short port_word_in(unsigned short port)
{
    unsigned short result;
    __asm__("inw %1, %0" : "=a" (result) : "Nd" (port));  // Fixed: inw for word operations
    return result;
}

// A function that puts a word into a specified port
void port_word_out(unsigned short port, unsigned short data)
{
    __asm__("outw %0, %1" : : "a" (data), "Nd" (port));  // Fixed: outw for word operations
}

// A function that puts a byte in a specified port
void outb(unsigned short port, unsigned char val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

// A function that reads a byte from a specified port
unsigned char inb(unsigned short port)
{
    char ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

// Copy bytes from a place to another
void mem_cpy(unsigned char *source, unsigned char *dest, int bytes)
{
	for (int i = 0; i < bytes; i++) {
		dest[i] = source[i];
	}
}
