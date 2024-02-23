#pragma once
#include <keyboard.h>
#include <video.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static void render();
static void setup();
static VGATextFrame frame;
static Cursor cursor;
static char buffer[3];
static int bufferIndex;
static KeyStroke keyStroke;
static bool killSignal;

static void setColors(){
    for (int i = 0; i < VGA_WIDTH; i++){
        frame.buffer[VGA_HEIGHT_BASE][i].background_color = VGA_COLOR_GREEN;
        frame.buffer[VGA_HEIGHT_BASE][i].foreground_color = VGA_COLOR_BLACK;
        frame.buffer[VGA_HEIGHT-1][i].background_color = VGA_COLOR_WHITE;
        frame.buffer[VGA_HEIGHT-1][i].foreground_color = VGA_COLOR_BLACK;
    }
};

static void render(){
    setColors();

    update_cursor(
        &cursor, 0,VGA_HEIGHT_BASE
    );
    printf("Keyboard Check");

    update_cursor(&cursor, 13, 12);
    printf("ASCII");
    update_cursor(&cursor, 13, 13);
    printf("%c",keyStroke.ascii);

    update_cursor(&cursor, 30, 12);
    printf("Key Code");
    update_cursor(&cursor, 30, 13);
    printf("%d",keyStroke.keyCode);

    update_cursor(&cursor, 45, 12);
    printf("Key Release\n");
    update_cursor(&cursor, 45, 13);
    printf("%d",keyStroke.keyReleased);

    update_cursor(&cursor, 60, 12);
    printf("Scan Code");
    update_cursor(&cursor, 60, 13);
    printf("%d",keyStroke.scanCode);

    update_cursor(&cursor, 0, VGA_HEIGHT-1);
    printf("command: %s (to quit please type \"./\")", buffer);
    
    videoInterruptHandler();
}

static void keyboardCallback(KeyStroke ks){
    if (ks.keyReleased) return;
    keyStroke = ks;
    if(ks.ascii != NULL) {
        buffer[bufferIndex] = ks.ascii;
        bufferIndex = (bufferIndex + 1) % 2;
    }
    if(buffer[0] == '.' && buffer[1] == 'q') killSignal = true;
    render();
}

void keypress(){
    frameFill(&frame,VGA_COLOR_BLACK,VGA_COLOR_GREEN);
    
    bufferIndex = 0;
    buffer[0] = 0;
    buffer[1] = 0;
    killSignal = false;
    memset(&buffer, '\0', 3);
    setKeyboardCallback(&keyboardCallback);
    frameFill(&frame, VGA_COLOR_BLACK, VGA_COLOR_GREEN);
    update_cursor(&cursor, 0,2);
    requestVideoOut(&frame,&cursor);

    while (!killSignal){
        for (int i = 0; i < 100000; i++)
        {
            /* code */
        }
        
        render();
    }
}