#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kernel/include/tools.h"
#include "kernel/include/terminal.h"
#include "kernel/include/GDT.h"
#include "kernel/include/interrupts.h"
#include <stdio.h>


/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

 


void kernel_main(void) 
{
	setGdt(0x08, 0x00000000);
	idt_initialize();
	enableInterrupts();
    terminal_initialize();
	write_string("1 OS\n", vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK) , get_terminal_cursor());
	printf("Hello, kernel World %d!\n", -999);
	printf("Hello, kernel World %d!\n", 55 + 2);
	printf("Hello, kernel World %d!\n", 32 * 2);
	printf("%s Hello, kernel World %d!\n","firat", 2 + 2);
}

