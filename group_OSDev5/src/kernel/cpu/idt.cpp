#include "idt.h"
#include "screen.h"
//#include <memory.h>
#include "hardware_port.h"

void init_idt() asm ("init_idt");
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// Lets us access our ASM functions from our C code.
extern "C"{
    void idt_flush(uint32_t);

    // ISRs reserved for CPU exceptions
    extern void isr0 ();
    extern void isr1 ();
    extern void isr2 ();

    // IRQs reserved for hardware interrupts
    extern void irq0 ();
    extern void irq1 ();
    extern void irq2 ();
    extern void irq3 ();
    extern void irq4 ();
    extern void irq5 ();
    extern void irq6 ();
    extern void irq7 ();
    extern void irq8 ();
    extern void irq9 ();
    extern void irq10();
    extern void irq11();
    extern void irq12();
    extern void irq13();
    extern void irq14();
    extern void irq15();
}

// This exists in 'start.asm', and is used to load our IDT.
idt_entry_t idt_entries[NUM_IDT_ENTRIES];

// The IDT itself.
idt_ptr_t   idt_ptr;

// Use this function to set an entry in the IDT. A lot simpler than twiddling with the GDT ;)
void init_idt()
{
    print("Initializing IDT...");
    idt_ptr.limit = sizeof(idt_entry_t) * NUM_IDT_ENTRIES -1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    //memset(&idt_entries, 0, sizeof(idt_entry_t)*NUM_IDT_ENTRIES);
     for(int i = 0; i < NUM_IDT_ENTRIES; ++i) {
        idt_entries[i].base_lo = 0;
        idt_entries[i].base_hi = 0;
        idt_entries[i].sel = 0;
        idt_entries[i].always0 = 0;
        idt_entries[i].flags = 0;
    }

    // Remap the irq table.
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    // Install the IRQs
    idt_set_gate(0, (uint32_t)isr0 , 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1 , 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr2 , 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)irq1, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)irq2, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)irq3, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)irq4, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)irq5, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)irq6, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)irq7, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)irq8, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)irq9, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)irq15, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
    print("[x] IDT initialized");
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags   = flags  | 0x60;
}