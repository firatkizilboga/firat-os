#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "terminal.h"
#include "video.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <keypress.h>
#include <editor.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
void disable_timer_interrupt() {
  unsigned char mask;

  // Read the current mask
  asm volatile("inb %1, %0" : "=a"(mask) : "Nd"(PIC1_DATA));

  // Set the mask for IRQ0
  mask |= 0x01;

  // Write the new mask
  outb(PIC1_DATA, mask);
}

#define DATA_PORT 0x60
#define ACK 0xFA
#define KEYBOARD_DATA_PORT 0x60
#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0
#define PIC2_DATA       0xA1

#define PIC_EOI         0x20

#define ICW1_ICW4       0x01
#define ICW1_INIT       0x10
#define ICW4_8086       0x01

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

void PIC_sendEOI(unsigned char irq) {
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);

    outb(PIC1_COMMAND, PIC_EOI);
}

void initializePIC() {
    // Remap the PIC to avoid conflicts with CPU exceptions
    // IRQ 0..15 will be remapped to INT 0x20..0x2F
    PIC_remap(0x20, 0x28);
}

int main() {
    initializePIC();
    // Rest of your initialization code...
    return 0;
}


void kernel_main(void) {
  initializePIC();
  i686_GDT_Initialize();
  i686_IDT_Initialize();
  enableInterrupts();
  disableAllInterruptGates();
  disable_timer_interrupt();
  initVideo();
  initKeyboard();
  terminal();
}
