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

const uint32_t UNKNOWN = 0xFFFFFFFF;
const uint32_t ESC = 0xFFFFFFFF - 1;
const uint32_t CTRL = 0xFFFFFFFF - 2;
const uint32_t LSHFT = 0xFFFFFFFF - 3;
const uint32_t RSHFT = 0xFFFFFFFF - 4;
const uint32_t ALT = 0xFFFFFFFF - 5;
const uint32_t F1 = 0xFFFFFFFF - 6;
const uint32_t F2 = 0xFFFFFFFF - 7;
const uint32_t F3 = 0xFFFFFFFF - 8;
const uint32_t F4 = 0xFFFFFFFF - 9;
const uint32_t F5 = 0xFFFFFFFF - 10;
const uint32_t F6 = 0xFFFFFFFF - 11;
const uint32_t F7 = 0xFFFFFFFF - 12;
const uint32_t F8 = 0xFFFFFFFF - 13;
const uint32_t F9 = 0xFFFFFFFF - 14;
const uint32_t F10 = 0xFFFFFFFF - 15;
const uint32_t F11 = 0xFFFFFFFF - 16;
const uint32_t F12 = 0xFFFFFFFF - 17;
const uint32_t SCRLCK = 0xFFFFFFFF - 18;
const uint32_t HOME = 0xFFFFFFFF - 19;
const uint32_t UP = 0xFFFFFFFF - 20;
const uint32_t LEFT = 0xFFFFFFFF - 21;
const uint32_t RIGHT = 0xFFFFFFFF - 22;
const uint32_t DOWN = 0xFFFFFFFF - 23;
const uint32_t PGUP = 0xFFFFFFFF - 24;
const uint32_t PGDOWN = 0xFFFFFFFF - 25;
const uint32_t END = 0xFFFFFFFF - 26;
const uint32_t INS = 0xFFFFFFFF - 27;
const uint32_t DEL = 0xFFFFFFFF - 28;
const uint32_t CAPS = 0xFFFFFFFF - 29;
const uint32_t NONE = 0xFFFFFFFF - 30;
const uint32_t ALTGR = 0xFFFFFFFF - 31;
const uint32_t NUMLCK = 0xFFFFFFFF - 32;


const uint32_t lowercase[128] = {
UNKNOWN,ESC,'1','2','3','4','5','6','7','8',
'9','0','-','=','\b','\t','q','w','e','r',
't','y','u','i','o','p','[',']','\n',CTRL,
'a','s','d','f','g','h','j','k','l',';',
'\'','`',LSHFT,'\\','z','x','c','v','b','n','m',',',
'.','/',RSHFT,'*',ALT,' ',CAPS,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,NUMLCK,SCRLCK,HOME,UP,PGUP,'-',LEFT,UNKNOWN,RIGHT,
'+',END,DOWN,PGDOWN,INS,DEL,UNKNOWN,UNKNOWN,UNKNOWN,F11,F12,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN
};

const uint32_t uppercase[128] = {
    UNKNOWN,ESC,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','Q','W','E','R',
'T','Y','U','I','O','P','{','}','\n',CTRL,'A','S','D','F','G','H','J','K','L',':','"','~',LSHFT,'|','Z','X','C',
'V','B','N','M','<','>','?',RSHFT,'*',ALT,' ',CAPS,F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,NUMLCK,SCRLCK,HOME,UP,PGUP,'-',
LEFT,UNKNOWN,RIGHT,'+',END,DOWN,PGDOWN,INS,DEL,UNKNOWN,UNKNOWN,UNKNOWN,F11,F12,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,
UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN,UNKNOWN
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

    switch (scanCode) {
        // Handling special keys like function keys, control, alt, etc.
        case ESC ... ALTGR: // Using range in switch-case for compactness
            break;

        case LSHFT:
        case RSHFT:
            capsOn = !keyReleased;
            break;

        case 58:
            if (!keyReleased) {
                capsLock = !capsLock;
            }
            break;

        default:
            if (!keyReleased) {
                keyToPrint = capsOn || capsLock ? uppercase[scanCode] : lowercase[scanCode];
                if (keyToPrint != UNKNOWN) {} else {

                }
            }
    }

    outb(0x20, PIC1_COMMAND);
    enableInterrupts();
    KeyStroke keyStroke = {scanCode, keyReleased, keyToPrint};
    keyboardCallback(keyStroke);
    return;
}

void basicKeyboardCallback(KeyStroke keyStroke) {
    return;
}

void initKeyboard(){
    capsOn = false;
    capsLock = false;
	i686_IDT_SetGate(8, keyboardHandler, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
	i686_IDT_SetGate(9, keyboardHandler, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    setKeyboardCallback(basicKeyboardCallback);
}

