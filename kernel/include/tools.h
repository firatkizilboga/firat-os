#ifndef GDT_H
#define GDT_H

#include <stdint.h> // For standard integer types
#include <stddef.h> // For size_t
#include <stdbool.h> // For boolean types

// Structure representing a GDT entry
struct GDT;

// Function to encode a GDT entry
void encodeGdtEntry(uint8_t *target, struct GDT source);

// Function to disable interrupts
void disableInterrupts();

// Function to enable interrupts
void enableInterrupts();

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
uint16_t vga_entry(unsigned char uc, uint8_t color);
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

bool is_protected_mode();

#endif // GDT_H

