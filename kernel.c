#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kernel/include/tools.h"
#include "kernel/include/terminal.h"
#include "kernel/include/gdt.h"
#include "kernel/include/idt.h"
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

 
__attribute__((noreturn))
void halt_cpu_isr();

// In your initialization code
void setup_halt_interrupt() {
    // Set up an interrupt gate, for example, at interrupt vector 0x21 (33 in decimal)
    i686_IDT_SetGate(33, halt_cpu_isr, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);

}

void kernel_main(void) 
{
    terminal_initialize();
	i686_GDT_Initialize();
	i686_IDT_Initialize();
	enableInterrupts();
	setup_halt_interrupt();


	write_string("Firat OS\n", vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK) , get_terminal_cursor());
	printf("Hello, kernel World %d\n", -999);
	printf("Hello, kernel World %d\n", 55 + 2);
	__asm__ volatile ("int $0x21");
	printf("Hello, kernel World %d\n", 32 * 2);
	printf("Hello, kernel World %d\n", 2 + 2);

	printf("add two numbers %d!\n", add_two(2, 2));
}


 // ISR that halts the CPU
__attribute__((noreturn))
void halt_cpu_isr() {
	printf("Halt this");
    __asm__ volatile ("cli; hlt");
}