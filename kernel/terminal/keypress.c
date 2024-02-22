#pragma once
#include <stddef.h>
#include <stdint.h>
#include "stdbool.h"
#include "keyboard.h"
#include "terminal.h"
#include "tools.h"
#include "stdio.h"
#include "string.h"


KeyStroke incomingKey;
Cursor * c;
char buffer[2] = {0, 0};
int bufferIndex = 0;
bool killSignal = false;

void keypressKeyboardCallback(KeyStroke ks){
    incomingKey = ks;

    if(bufferIndex > 1 || bufferIndex < 0){
        bufferIndex = 0;
    }
    
    if(ks.ascii != NULL){
        buffer[bufferIndex] = ks.ascii;
        bufferIndex = (bufferIndex + 1) % 2;
    }

    if (buffer[0] == '.' && buffer[1] == '/'){
        killSignal = true;
    }
    render();
}

initialize_program(){  
    buffer[0] = 0;
    buffer[1] = 0;
    bufferIndex = 0;
    killSignal = false;
    setKeyboardCallback(&keypressKeyboardCallback);
    c = get_terminal_cursor();
    render();
}

render(){
    update_cursor(c, 0, 2);

    write_string("Press a key\n", vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_GREEN), c);
    printf("Key: %c\n", incomingKey.ascii);
    printf("Scan code: %d\n", incomingKey.scanCode);
    printf("Key released: %d\n", incomingKey.keyReleased);

    update_cursor(c, 0, 24);
    write_string("command buffer: ", vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE), c);
    write_string(buffer, vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE), c);
    write_string("type \"./\" to quit.\n", vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE), c);
}

void keypress(){
    initialize_program();

    while(!killSignal){
        render();
    }
}