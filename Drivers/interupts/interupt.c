#include <stdbool.h>
#include "../keybord.h"
#include "interupt.h"
#include "../screen.h"
#include "../low_level.h"


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

	// Initialize the PIC
    // ICW1: Start initialization sequence
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    
    // ICW2: Set interrupt vector offsets
    outb(0x21, 0x20); // Master PIC: IRQs 0-7 mapped to vectors 32-39
    outb(0xA1, 0x28); // Slave PIC: IRQs 8-15 mapped to vectors 40-47
    
    // ICW3: Configure master/slave relationship
    outb(0x21, 0x04); // Master PIC: Slave connected to IRQ2
    outb(0xA1, 0x02); // Slave PIC: Cascade identity
    
    // ICW4: Set mode
    outb(0x21, 0x01); // 8086 mode
    outb(0xA1, 0x01); // 8086 mode
    
    // Mask all interrupts except keyboard (IRQ1)
    outb(0x21, 0xFD); // Master PIC: Only IRQ1 unmasked
    outb(0xA1, 0xFF); // Slave PIC: All masked

	// Set up keyboard interrupt (IRQ1 -> vector 33)
    idt_set_descriptor(33, (void*)keyboard_irq_handler, 0x8E);
    vectors[33] = true;

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}
