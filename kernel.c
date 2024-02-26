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
#include <timer.h>

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


void kernel_main(void) {
    maskIRQ(0);
    disableInterrupts();
    i686_GDT_Initialize();
    i686_IDT_Initialize();
    initVideo();
    initKeyboard();
	  initTimer();
    enableInterrupts();
    terminal();
}
