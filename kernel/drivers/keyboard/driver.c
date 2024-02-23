#pragma once
#include "stdint.h"
#include "stdbool.h"
#include "keyboard.h"
#include "idt.h"
#include "terminal.h"
#include "stdio.h"
#define PIC1_COMMAND 0x20
#define KEYBOARD_DATA_PORT    0x60

bool capsOn;
bool capsLock;



const uint32_t lowercase[128] = {
NULL,NULL,'1','2','3','4','5','6','7','8',
'9','0','-','=',NULL,'\t','q','w','e','r',
't','y','u','i','o','p','[',']','\n',NULL,
'a','s','d','f','g','h','j','k','l',';',
'\'','`',NULL,'\\','z','x','c','v','b','n','m',',',
'.','/',NULL,'*',NULL,' ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'-',NULL,NULL,NULL,
'+',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL
};

const uint32_t uppercase[128] = {
    NULL,NULL,'!','@','#','$','%','^','&','*','(',')','_','+',NULL,'\t','Q','W','E','R',
'T','Y','U','I','O','P','{','}','\n',NULL,'A','S','D','F','G','H','J','K','L',':','"','~',NULL,'|','Z','X','C',
'V','B','N','M','<','>','?',NULL,'*',NULL,' ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,'-',
NULL,NULL,NULL,'+',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
NULL,NULL,NULL,NULL,NULL,NULL,NULL
};


void (*keyboardCallback)(KeyStroke);
void setKeyboardCallback(void (*callback)(KeyStroke)) {
    keyboardCallback = callback;
}


void keyboardHandler() {
    disableInterrupts();

    uint8_t scanCode = inb(KEYBOARD_DATA_PORT);
    bool keyReleased = scanCode & 0x80; // Check if the key is released
    scanCode &= 0x7F; // Extract the scan code

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
                keyToPrint = capsOn || capsLock ? uppercase[scanCode] : lowercase[scanCode];
                if (keyToPrint){
                    keyToPrint = keyToPrint;
                }
                else {
                    keyToPrint = NULL;
                }
            }
    }

    outb(0x20, PIC1_COMMAND);
    enableInterrupts();
    KeyStroke keyStroke = {scanCode, keyReleased, keyToPrint, keyCode};
    keyboardCallback(keyStroke);
    return;
}

void basicKeyboardCallback(KeyStroke keyStroke) {
}

void initKeyboard(){
    capsOn = false;
    capsLock = false;

    i686_IDT_EnableGate(8);
    i686_IDT_EnableGate(9);
	i686_IDT_SetGate(8, keyboardHandler, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
	i686_IDT_SetGate(9, keyboardHandler, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    setKeyboardCallback(&basicKeyboardCallback);
}
