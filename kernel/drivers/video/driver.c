#pragma once
#include "video.h"
#include <stdbool.h>

Cursor *current_cursor;
VGATextFrame *current_frame;

Cursor *getCurrentCursor() { return current_cursor; };
VGATextFrame *getCurrentFrame() { return current_frame; };

void requestVideoOut(VGATextFrame *inc_frame, Cursor *inc_cursor) {
  current_frame = inc_frame;
  current_cursor = inc_cursor;
};

uint16_t *vga_buffer;
void initVideo() { 
  vga_buffer = (uint16_t *)0xB8000;
  frameFill(current_frame, VGA_COLOR_BLACK, VGA_COLOR_WHITE);
}

inline void displayPixel(Pixel p, int y, int x) {
  uint16_t pos = y * VGA_WIDTH + x;
  uint8_t color = vga_entry_color(p.foreground_color, p.background_color);
  vga_buffer[pos] = vga_entry(p.ASCII, color);
}


void videoInterruptHandler() {
  for (int i = 0; i < VGA_HEIGHT; i++) {
    for (int j = 0; j < VGA_WIDTH; j++) {
      displayPixel(current_frame->buffer[i][j], i, j);
    }
  }
}


void update_cursor(Cursor *c, int x, int y) {
  // set start y from 2s to 25
  if (y < VGA_HEIGHT_BASE) {
    y = VGA_HEIGHT_BASE;
    x = 0;
  }

  if (y >= VGA_HEIGHT) {
    y = VGA_HEIGHT;
    x = 0;
  }

  c->x = x;
  c->y = y;

  if (c->is_blinking) {
    uint16_t pos = y * VGA_WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
  } else {
    disable_cursor();
  }
}

void write_char(char c, Cursor *cursor, VGATextFrame *frame) {
  if (c == '\n') {
    update_cursor(cursor, 0, cursor->y + 1);

  } else {
    frame->buffer[cursor->y][cursor->x].ASCII = c;
    update_cursor(cursor, cursor->x + 1, cursor->y);
  }
}

void write_string(char *str, Cursor *cursor, VGATextFrame *frame) {
  for (size_t i = 0; str[i] != '\0'; i++)
    write_char(str[i], cursor, frame);
}

void frameFill(VGATextFrame *frame, uint8_t background_color, uint8_t foreground_color) {

  for (int i = 0; i < VGA_HEIGHT; i++) {
    for (int j = 0; j < VGA_WIDTH; j++) {
      Pixel p = {background_color, foreground_color, ' '};
      frame->buffer[i][j] = p;
    }
  }
}

void disable_cursor() {
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);
}
