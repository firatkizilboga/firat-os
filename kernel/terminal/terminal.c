#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "video.h"
#include "keyboard.h"
#include "keypress.h"
#include "utils.h"
#include "terminal.h"
#include "editor.h"
#include "timer.h"

static void keyboardCallback(KeyStroke ks);
Cursor terminal_cursor = {0, 2, false};
static VGATextFrame frame;

void terminal_clear()
{
	terminal_cursor.x = VGA_WIDTH/2;
	terminal_cursor.y = VGA_HEIGHT/2;
	terminal_cursor.x = 0;
	terminal_cursor.y = 2;
	terminal_cursor.is_blinking = false;
	frameFill(&frame, VGA_COLOR_BLACK, VGA_COLOR_WHITE);
};

static char input_buffer[256];
static char data_buffer[20][256];
static int data_buffer_index = 0;
static int input_buffer_index = 0;

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

static void keyboardCallback(KeyStroke ks){
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
			keypress();
		}
		else if (strlen(input_buffer)>1){
			memcpy(data_buffer[data_buffer_index], input_buffer, 256);
			data_buffer_index++;
		}
		clearInputBuffer();
	}

	else if (ks.scanCode == KEY_BACKSPACE){
		if (input_buffer_index > 0)
		{
			input_buffer_index--;
			input_buffer[input_buffer_index] = '\0';
		}
	}
	render_terminal();
};

void render_terminal(){
	setKeyboardCallback(&keyboardCallback);
	requestVideoOut(&frame, &terminal_cursor);
	frameFill(&frame, VGA_COLOR_BLACK, VGA_COLOR_GREEN);
	update_cursor(&terminal_cursor, 0, 2);
	for (int i = 0; i < 20; i++){
		if (data_buffer[i][0] == '\0')
			break;
		write_string(data_buffer[i], &terminal_cursor, &frame);
	}
	update_cursor(&terminal_cursor, 0, 24);
	write_string(">>>", &terminal_cursor, &frame);
	if (input_buffer_index)
	{

		write_string(input_buffer, &terminal_cursor, &frame);
	}
	while(terminal_cursor.x < VGA_WIDTH)
	{
		write_char(' ', &terminal_cursor, &frame);
	}
	videoInterruptHandler();
}

void terminal_initialize(void) 
{
	requestVideoOut(&frame, &terminal_cursor);
	frameFill(&frame, VGA_COLOR_BLACK,VGA_COLOR_GREEN);
	clearBuffers();
	render_terminal();
}

void terminal(){
	//write \0 to data_buffer and input_buffer
	terminal_initialize();
	while (1)
	{}
}