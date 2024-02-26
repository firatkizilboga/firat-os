#pragma once
#include "idt.h"
#include <stdint.h>
#define FLAG_SET(x, flag) x |= (flag)
#define FLAG_UNSET(x, flag) x &= ~(flag)
#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0
#define PIC2_DATA       0xA1

#define PIC_EOI         0x20

#define ICW1_ICW4       0x01
#define ICW1_INIT       0x10
#define ICW4_8086       0x01

typedef struct
{
    uint16_t BaseLow;
    uint16_t SegmentSelector;
    uint8_t Reserved;
    uint8_t Flags;
    uint16_t BaseHigh;
} __attribute__((packed)) IDTEntry;

typedef struct
{
    uint16_t Limit;
    IDTEntry* Ptr;
} __attribute__((packed)) IDTDescriptor;


IDTEntry g_IDT[256];

IDTDescriptor g_IDTDescriptor = { sizeof(g_IDT) - 1, g_IDT };

void __attribute__((cdecl)) i686_IDT_Load(IDTDescriptor* idtDescriptor);

void i686_IDT_SetGate(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags)
{
    g_IDT[interrupt].BaseLow = ((uint32_t)base) & 0xFFFF;
    g_IDT[interrupt].SegmentSelector = segmentDescriptor;
    g_IDT[interrupt].Reserved = 0;
    g_IDT[interrupt].Flags = flags;
    g_IDT[interrupt].BaseHigh = ((uint32_t)base >> 16) & 0xFFFF;
}

inline void i686_IDT_EnableGate(int interrupt)
{
    FLAG_SET(g_IDT[interrupt].Flags, IDT_FLAG_PRESENT);
}

inline void i686_IDT_DisableGate(int interrupt)
{
    FLAG_UNSET(g_IDT[interrupt].Flags, IDT_FLAG_PRESENT);
}

void i686_IDT_Initialize()
{
    i686_IDT_Load(&g_IDTDescriptor);
    initializePIC();
}

void outb(uint16_t port, uint8_t val) {
    __asm__("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void PIC_remap(int offset1, int offset2) {
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);                        // save masks
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    outb(PIC1_DATA, offset1);                   // ICW2: Master PIC vector offset
    outb(PIC2_DATA, offset2);                   // ICW2: Slave PIC vector offset

    outb(PIC1_DATA, 4);                         // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(PIC2_DATA, 2);                         // ICW3: tell Slave PIC its cascade identity (0000 0010)

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, a1);                        // restore saved masks
    outb(PIC2_DATA, a2);
}

inline void PIC_sendEOI(unsigned char irq) {
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);

    outb(PIC1_COMMAND, PIC_EOI);
}

void initializePIC() {
    PIC_remap(0x20, 0x28);
}

void maskIRQ(unsigned char IRQ) {
    unsigned short port;
    unsigned char value;

    if (IRQ < 8) {
        port = 0x21;  // Master PIC IMR port
    } else {
        port = 0xA1;  // Slave PIC IMR port
        IRQ -= 8;
    }

    value = inb(port) | (1 << IRQ);
    outb(port, value);
}

void unmaskIRQ(unsigned char IRQ) {
    unsigned short port;
    unsigned char value;

    if (IRQ < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQ -= 8;
    }

    value = inb(port) & ~(1 << IRQ);
    outb(port, value);
}

inline void enableInterrupts(){
    __asm__ __volatile__("sti");
}

inline void disableInterrupts(){
    __asm__ __volatile__("cli");
}

inline void disableAllInterruptGates(){
    for (int i = 0; i < 256; i++)
    {
        i686_IDT_DisableGate(i);
    }
}

__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}

