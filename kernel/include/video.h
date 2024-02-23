#ifndef video_H
#define video_H 1

#pragma once
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_HEIGHT_BASE 2

#include <stdint.h> // For standard integer types
#include <stddef.h> // For size_t
#include <stdbool.h> // For boolean types

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

typedef struct{
    uint16_t background_color;
    uint16_t foreground_color;
    char ASCII;
} Pixel;

typedef struct {
    Pixel buffer[VGA_HEIGHT][VGA_WIDTH];
} VGATextFrame;

typedef struct {
	size_t x;
	size_t y;
	bool is_blinking;
} Cursor;

void requestVideoOut(VGATextFrame*, Cursor*);
void write_char(char c, Cursor * cursor, VGATextFrame * frame);
void write_string(char * str, Cursor * cursor, VGATextFrame * frame);
void frameFill(VGATextFrame* frame, uint8_t background_color, uint8_t foreground_color);
void videoInterruptHandler();
Cursor * getCurrentCursor();
VGATextFrame * getCurrentFrame();
void initVideo();
void update_cursor(Cursor * c, int x, int y);
#endif
