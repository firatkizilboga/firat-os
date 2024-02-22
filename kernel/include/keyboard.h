#ifndef KEYBOARD_H
#define KEYBOARD_H 1
typedef struct {
    uint8_t scanCode;
    bool keyReleased;
    char ascii;
} KeyStroke;

void initKeyboard();
void setKeyboardCallback(void (*callback)(KeyStroke));
#endif