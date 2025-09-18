#include "../low_level/low_level.h"
#include "PIC.h"

// Initialize the PIC
void init_pic(void)
{
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
}