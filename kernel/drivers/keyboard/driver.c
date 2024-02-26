#pragma once
#include "idt.h"
#include "keyboard.h"
#include "stdbool.h"
#include "stdint.h"
#include <stddef.h>
#define KEYBOARD_DATA_PORT 0x60

static bool capsOn;
static bool capsLock;

const uint32_t lowercase[128] = {
    NULL, NULL, '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',
    '-',  '=',  NULL, '\t', 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',
    'o',  'p',  '[',  ']',  '\n', NULL, 'a',  's',  'd',  'f',  'g',  'h',
    'j',  'k',  'l',  ';',  '\'', '`',  NULL, '\\', 'z',  'x',  'c',  'v',
    'b',  'n',  'm',  ',',  '.',  '/',  NULL, '*',  NULL, ' ',  NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, '-',  NULL, NULL, NULL, '+',  NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

const uint32_t uppercase[128] = {
    NULL, NULL, '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',
    '_',  '+',  NULL, '\t', 'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',
    'O',  'P',  '{',  '}',  '\n', NULL, 'A',  'S',  'D',  'F',  'G',  'H',
    'J',  'K',  'L',  ':',  '"',  '~',  NULL, '|',  'Z',  'X',  'C',  'V',
    'B',  'N',  'M',  '<',  '>',  '?',  NULL, '*',  NULL, ' ',  NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, '-',  NULL, NULL, NULL, '+',  NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

static void (*keyboardCallback)(KeyStroke);
void setKeyboardCallback(void (*callback)(KeyStroke)) {
  keyboardCallback = callback;
}

void keyboardHandler() {
  disableInterrupts();

  uint8_t scanCode = inb(KEYBOARD_DATA_PORT);
  bool keyReleased = scanCode & 0x80; // Check if the key is released
  scanCode &= 0x7F;                   // Extract the scan code

  // Debugging information
  char keyToPrint = NULL;

  KeyCode keyCode = (KeyCode)scanCode;
  switch (keyCode) {
  case KEY_LSHFT:
  case KEY_RSHFT:
    capsOn = !keyReleased;
    break;

  case KEY_CAPS:
    if (!keyReleased) {
      capsLock = !capsLock;
    }
    break;

  default:
    if (!keyReleased) {
      keyToPrint =
          capsOn || capsLock ? uppercase[scanCode] : lowercase[scanCode];
      if (keyToPrint) {
        keyToPrint = keyToPrint;
      } else {
        keyToPrint = NULL;
      }
    }
  }
  PIC_sendEOI(0x21);
  enableInterrupts();
  KeyStroke keyStroke = {scanCode, keyReleased, keyToPrint, keyCode};
  keyboardCallback(keyStroke);
  return;
}

void basicKeyboardCallback(KeyStroke keyStroke) {}

void initKeyboard() {
  capsOn = false;
  capsLock = false;

  i686_IDT_EnableGate(0x21);
  i686_IDT_SetGate(0x21, keyboardHandler, 0x08,
                   IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
}
