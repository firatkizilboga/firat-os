#pragma once
#include <editor.h>
#include <keyboard.h>
#include <stdio.h>
#include <string.h>
#include <video.h>

static void render();
static void setup();
static VGATextFrame frame;
static Cursor cursor;
static char buffer[(VGA_HEIGHT - VGA_HEIGHT_BASE) * VGA_WIDTH];
static int bufferIndex;
static void clear() {
  frameFill(&frame, VGA_COLOR_BLUE, VGA_COLOR_WHITE);
  update_cursor(&cursor, 0, VGA_HEIGHT_BASE);
};

static void render() {
  clear();
  write_string(buffer, &cursor, &frame);
  videoInterruptHandler();
}

static void keyboardCallback(KeyStroke ks) {
  if (ks.keyReleased)
    return;

  if (ks.keyCode == KEY_BACKSPACE) {
    bufferIndex--;
    buffer[bufferIndex] = '\0';
  } else if (ks.ascii != NULL) {
    buffer[bufferIndex] = ks.ascii;
    bufferIndex++;
    if (bufferIndex >= (VGA_HEIGHT - VGA_HEIGHT_BASE) * VGA_WIDTH) {
      bufferIndex = 0;
    }
  }
  render();
}

void editor() {
  bufferIndex = 0;
  memset(&buffer, '\0', 23 * 80);
  setKeyboardCallback(&keyboardCallback);

  frameFill(&frame, VGA_COLOR_BLACK, VGA_COLOR_WHITE);
  update_cursor(&cursor, 0, 2);
  requestVideoOut(&frame, &cursor);

  render();
  while (1) {
  }
}
