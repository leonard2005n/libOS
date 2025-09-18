#include <stdbool.h>
#include "../low_level/low_level.h"
#include "../keyboard/keyboard.h"
#include "../screen/screen.h"
#include "interupt.h"



extern void keyboard_irq_handler(void);
idt_entry_t idt[256];
idtr_t idtr;
static bool vectors[IDT_MAX_DESCRIPTORS];
extern void* isr_stub_table[];

void exception_handler(void);
void exception_handler() {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}

// Function that sets the idt
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags)
{
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

void idt_init()
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

	// Set up keyboard interrupt (IRQ1 -> vector 33)
    idt_set_descriptor(33, (void*)keyboard_irq_handler, 0x8E);
    vectors[33] = true;

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}
