#pragma once
#define PIT_FREQ 1193182
#define TIMER_HZ 20
#include <stdint.h>
#include <idt.h>
#include <video.h>
#include <idt.h>


#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
static uint64_t ticks;
static const uint32_t freq = PIT_FREQ;

int getTicks(){
    return ticks;
};


static void onTimerInterrupt() {
    disableInterrupts();
    ticks++;
    videoInterruptHandler();
    PIC_sendEOI(0);
    enableInterrupts();
}

int double_fault(){
    disableInterrupts();
    Cursor* c = getCurrentCursor();
    VGATextFrame* f = getCurrentFrame();
    update_cursor(c, 40,20);
    write_string("double fault!", c, f);
    enableInterrupts();
    return 1;
}

void initTimer(){
    uint16_t divisor = PIT_FREQ / TIMER_HZ ;

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
    
    outb(0x43, 0x36); // Command byte: 0x36 = 00 11 011 0
    outb(0x40, low);
    outb(0x40, high);

    i686_IDT_EnableGate(0x20);
    i686_IDT_SetGate(0x20, onTimerInterrupt, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    i686_IDT_EnableGate(8);
    i686_IDT_SetGate(8, double_fault, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    unmaskIRQ(0);   
}