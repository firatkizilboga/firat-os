#pragma once
#include <stddef.h>
#include <stdint.h>
#include "video.h"
#include <stdbool.h>
#include <string.h>
#include "keyboard.h"
#include "keypress.h"

void terminalKeyboardCallback(KeyStroke ks);

/* Hardware text mode color constants. */ 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void outb(uint16_t Port, uint8_t Value){
	asm volatile ("outb %1, %0" : : "dN" (Port),"a" (Value));
}

char inb(uint16_t port){
	char rv;
	asm volatile("inb %1, %0": "=a" (rv): "dN" (port));
	return rv;
}

typedef struct {
	size_t x;
	size_t y;
	bool is_blinking;
} Cursor;

Cursor terminal_cursor = {0, 2, false};

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
	disable_cursor();
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

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}


char input_buffer[256];
char data_buffer[20][256];
int data_buffer_index = 0;
int input_buffer_index = 0;

void clearInputBuffer(){
	memset(input_buffer, '\0', 256);
	input_buffer_index = 0;
}

void clearDataBuffer(){
	data_buffer_index = 0;
	for (int i = 0; i < 20; i++)
	{
		memset(data_buffer[i], 0, 256);
	}
}

void clearBuffers()
{
	clearInputBuffer();
	clearDataBuffer();	
}

void terminalKeyboardCallback(KeyStroke ks){
	if (ks.keyReleased)
		return;
	
	if (ks.ascii != NULL){
		input_buffer[input_buffer_index] = ks.ascii;
		input_buffer_index++;
	}

	if (ks.ascii == '\n'){
		input_buffer[input_buffer_index] = '\0';
		input_buffer_index = 0;
		if (data_buffer_index >= 20)
		{
			for (int i = 0; i < 20; i++)
			{
				memcpy(data_buffer[i], data_buffer[i + 1], 256);
			}
			data_buffer_index = 20;
		}

		if (input_buffer[0] == 'c')
		{
			clearBuffers();
		} else if (input_buffer[0] == 'q')
		{
			terminal_clear(&terminal_cursor);
			keypress();
			clearInputBuffer();
		}
		else if (strlen(input_buffer)>1){
			memcpy(data_buffer[data_buffer_index], input_buffer, 256);
			clearInputBuffer();
			data_buffer_index++;
		}
	}

	else if (ks.scanCode == (uint16_t) 14){
		if (input_buffer_index > 0)
		{
			input_buffer_index--;
			input_buffer[input_buffer_index] = '\0';
		}
	}

	render_terminal();
};

render_terminal(){
	terminal_clear(&terminal_cursor);
	setKeyboardCallback(&terminalKeyboardCallback);

	
	for (int i = 0; i < 20; i++)
	{
		if (data_buffer[i][0] == '\0')
			break;
		
		write_string(data_buffer[i], terminal_color, &terminal_cursor);
	}
	update_cursor(&terminal_cursor, 0, 24);
	write_string("prompt> ", terminal_color, &terminal_cursor);
	write_string(input_buffer, terminal_color, &terminal_cursor);
}

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	input_buffer_index = 0;
	data_buffer_index = 0;
	clearBuffers();
	render_terminal();
}
void terminal(){
	//write \0 to data_buffer and input_buffer
	terminal_initialize();
	while (1)
	{
		
	}
}


