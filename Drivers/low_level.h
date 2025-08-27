#pragma once

// A function that read a word from a specified port
unsigned short port_word_in (unsigned short port);

// A function that to put a word into a specified port
void port_word_out (unsigned short port, unsigned short data);

// A function that reads a byte form a spcified port
unsigned char inb(unsigned short port);

// A function that puts a byte in a specified port
void outb(unsigned short port, unsigned char val);

// Copy bytes from a place to another
void mem_cpy(unsigned char *source, unsigned char *dest, int bytes);
