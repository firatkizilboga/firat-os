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

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

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
#define PIC1_COMMAND 0x20
#define KEYBOARD_DATA_PORT 0x60

void handler() {
    disableInterrupts();

	char scan_code = inb(KEYBOARD_DATA_PORT);

	// You can process the scan code here
	printf("Keyboard Interrupt: %d\n", scan_code);
	outb(0x20, PIC1_COMMAND);
    enableInterrupts();
}

void kernel_main(void) 
{
	i686_GDT_Initialize();
	i686_IDT_Initialize();
	enableInterrupts();

	disable_timer_interrupt();
    terminal_initialize();
	
	write_string("Firat OS\n", vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK) , get_terminal_cursor());
	printf("Hello, kernel World %d\n", -999);
	printf("Hello, kernel World %d\n", 55 + 2);
	printf("Hello, kernel World %d\n", 32 * 2);
	printf("Hello, kernel World %d\n", 2 + 2);


	i686_IDT_SetGate(8, handler, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
	i686_IDT_SetGate(9, handler, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
	

	while (1)
	{
		for (int i = 0; i < 256000000; i++)
		{
		}
		printf("cycle\n");
		enableInterrupts();

		
	}
}