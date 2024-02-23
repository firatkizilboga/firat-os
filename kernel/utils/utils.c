#pragma once

#include <stdint.h>

int is_protected_mode() {
    uint32_t cr0;
    __asm__("mov %%cr0, %0" : "=r"(cr0)); // Move the value of CR0 into the variable cr0
    return cr0 & 0x1; // Check the PE bit (bit 0) of CR0
}

void outb(uint16_t Port, uint8_t Value){
	asm volatile ("outb %1, %0" : : "dN" (Port),"a" (Value));
}

char inb(uint16_t port){
	char rv;
	asm volatile("inb %1, %0": "=a" (rv): "dN" (port));
	return rv;
}