#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H 1

void enableInterrupts(){
    __asm__ __volatile__("sti");
}

void disableInterrupts(){
    __asm__ __volatile__("cli");
}

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance


typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;

__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
    __asm__ __volatile__("cli;");

}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];
 
    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

extern void idt_load(idtr_t* idtr);

void idt_initialize() {
    idt_set_descriptor(0x21, exception_handler, 0x8E); // 0x8E = Present, DPL=0, Type=14 (32-bit interrupt gate)
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t)&idt;

    idt_load(&idtr); // Load the IDT
}

#endif