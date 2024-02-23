#pragma once
#include<stdbool.h>
#ifndef KEYBOARD_H
#define KEYBOARD_H 1
typedef enum {
    KEY_UNKNOWN = 0xFFFFFFFF,
    KEY_BACKSPACE = 14,
    KEY_ESC = 1,
    KEY_TAB = 15,
    KEY_CAPS = 58,
    KEY_CTRL = 29,
    KEY_LSHFT = 42,
    KEY_RSHFT = 54,
    KEY_ALT = 56,
    KEY_WIN = 91,
    KEY_WIN2 = 92,
    KEY_ENTER = 28,
    KEY_F1 = 59,
    KEY_F2 = 60,
    KEY_F3 = 61,
    KEY_F4 = 62,
    KEY_F5 = 63,
    KEY_F6 = 64,
    KEY_F7 = 65,
    KEY_F8 = 66,
    KEY_F9 = 67,
    KEY_F10 = 68,
    KEY_F11 = 69,
    KEY_F12 = 88,
    KEY_SCRLCK = 18,
    KEY_HOME = 19,
    KEY_UP = 72,
    KEY_LEFT = 75,
    KEY_RIGHT = 77,
    KEY_DOWN = 80,
    KEY_PGUP = 24,
    KEY_PGDOWN = 25,
    KEY_END = 26,
    KEY_INS = 27,
    KEY_NONE = 30,
    KEY_ALTGR = 31,
    KEY_NUMLCK = 32
} KeyCode;

typedef struct {
    uint8_t scanCode;
    bool keyReleased;
    char ascii;
    KeyCode keyCode;
} KeyStroke;


void initKeyboard();
void setKeyboardCallback(void (*callback)(KeyStroke));
#endif