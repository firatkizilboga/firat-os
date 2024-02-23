#pragma once
#include <stdint.h>



bool is_protected_mode();
char inb(uint16_t port);
void outb(uint16_t port, uint8_t data);