#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "tools.h"
#include "terminal.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include <stdio.h>
#include <stdlib.h>



#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
void disable_timer_interrupt() {
    unsigned char mask;

    // Read the current mask
    asm volatile ("inb %1, %0" : "=a"(mask) : "Nd"(PIC1_DATA));

    // Set the mask for IRQ0
    mask |= 0x01;

    // Write the new mask
    outb(PIC1_DATA, mask);
}

#define DATA_PORT 0x60
#define ACK 0xFA
#define KEYBOARD_DATA_PORT 0x60
#define PIC1_COMMAND 0x20



void kernel_main(void) 
{
	i686_GDT_Initialize();
	i686_IDT_Initialize();
	enableInterrupts();

	disable_timer_interrupt();
	initKeyboard();
	terminal();
}