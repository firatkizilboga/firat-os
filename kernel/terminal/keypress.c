#pragma once
#include <stddef.h>
#include <stdint.h>
#include "stdbool.h"
#include "keyboard.h"
#include "video.h"
#include "stdio.h"
#include "string.h"
#include "keypress.h"

KeyStroke incomingKey;
char buffer[2] = {0, 0};
int bufferIndex = 0;
bool killSignal = false;

void keypressKeyboardCallback(KeyStroke ks){
    incomingKey = ks;
    
    if(ks.ascii != NULL && ks.keyCode != KEY_ENTER){
        bufferIndex = (bufferIndex + 1) % 2;
        buffer[bufferIndex] = ks.ascii;
    }

    if (buffer[0] == '.' && buffer[1] == '/'){
        killSignal = true;
    }
    render();
}

static VGATextFrame frame;
Cursor keypressCursor;


initialize_program(){  
    frameFill(&frame, VGA_COLOR_DARK_GREY, VGA_COLOR_CYAN);
    requestVideoOut(&frame, &keypressCursor);
    buffer[0] = 0;
    buffer[1] = 0;
    bufferIndex = 0;
    killSignal = false;
    setKeyboardCallback(&keypressKeyboardCallback);
    render();
}

render(){
    update_cursor(&keypressCursor, 0, 2);
    write_string("Press a key\n", &keypressCursor, &frame);
    printf("Key: %c\n", incomingKey.ascii);
    printf("Scan code: %d\n", incomingKey.scanCode);
    printf("Key released: %d\n", incomingKey.keyReleased);
    printf("KILL: \n");
    printf(killSignal ? "true": "false");
    printf("\n %d",bufferIndex);

    update_cursor(&keypressCursor, 0, 24);
    write_string("command buffer: ", &keypressCursor, &frame);
    write_string(buffer, &keypressCursor, &frame);
    write_string("type \"./\" to quit.\n", &keypressCursor, &frame);
    videoInterruptHandler();
    
}
void keypress(){
    initialize_program();

    while(!killSignal){
        for (size_t i = 0; i < 100; i++){}
        render();
    }
}