#include <stddef.h>
#include <stdint.h>
#include "../include/tools.h"
#include <stdbool.h>

/* Hardware text mode color constants. */ 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;


void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

typedef struct {
	size_t x;
	size_t y;
	bool is_blinking;
} Cursor;

Cursor terminal_cursor = {0, 2, true};

Cursor * get_terminal_cursor()
{
	return &terminal_cursor;
}
void update_cursor(Cursor * c, size_t x, size_t y)
{
	//set start y from 2s to 25
	if (y < 2)
	{
		y = 2;
		x = 0;
	}

	if (y >= 25)
	{
		y = 2;
		x = 0;
	}

    c->x = x;
    c->y = y;

	if (c->is_blinking)
	{
		uint16_t pos = y * VGA_WIDTH + x;
		outb(0x3D4, 0x0F);
		outb(0x3D5, (uint8_t) (pos & 0xFF));
		outb(0x3D4, 0x0E);
		outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
	}
}

void write_char(char c, uint8_t color, Cursor * cursor)
{
	uint16_t pos = cursor->y * VGA_WIDTH + cursor->x;
	
	if (c != '\n')
	{
		terminal_buffer[pos] = vga_entry(c, color);
		update_cursor(cursor, cursor->x + 1, cursor->y);
	}
	else
	{
		for (size_t i = cursor->x; i < VGA_WIDTH; i++)
		{
			pos = cursor->y * VGA_WIDTH +  i;
			terminal_buffer[pos] = vga_entry(' ', color);
		}
		update_cursor(cursor, 0, cursor->y + 1);
	}
}

void write_string(const char * str, uint8_t color, Cursor * cursor)
{
	for (size_t i = 0; str[i] != '\0'; i++)
		write_char(str[i], color, cursor);
}

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
    
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

 
void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void terminal_clear(Cursor * cursor)
{
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	update_cursor(cursor, 0, 2);
}
