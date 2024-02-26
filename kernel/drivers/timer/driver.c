#pragma once
#define PIT_FREQ 1193182
#define TIMER_HZ 50
#include <stdint.h>
#include <idt.h>
#include <video.h>
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
static uint64_t ticks;
static const uint32_t freq = PIT_FREQ;

static void (*timerInterruptHandler)(void);
void basicHandler(){};
int getTicks(){
    return ticks;
};

void setTimerInterruptHandler(void(*func)(void)){
    timerInterruptHandler = func;
}


static void onTimerInterrupt() {
    disableInterrupts();

    // Increment your tick count
    ticks += 1;
    timerInterruptHandler();
    // Acknowledge the interrupt to the PIC
    outb(0x20, 0x20); // Send EOI to the master PIC
    enableInterrupts();
}


void initTimer(){
    uint16_t divisor = PIT_FREQ / TIMER_HZ ;
    timerInterruptHandler = &basicHandler;
    // Send command byte to PIT Control Register

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(0x43, 0x36); // Command byte: 0x36 = 00 11 011 0
    outb(0x40, low);
    outb(0x40, high);

    char mask;
    asm volatile("inb %1, %0" : "=a"(mask) : "Nd"(PIC1_DATA));

    mask &= ~0x01;
    outb(PIC1_DATA, mask);

    // Set up IDT entry for timer interrupt
    for (size_t i = 128; i < 256; i++)
    {
        i686_IDT_EnableGate(i);
        i686_IDT_SetGate(i, videoInterruptHandler, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    }
    
}