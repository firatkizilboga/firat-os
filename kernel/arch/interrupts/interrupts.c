#pragma once
#include "idt.h"
void enableInterrupts(){
    __asm__ __volatile__("sti");
}

void disableInterrupts(){
    __asm__ __volatile__("cli");
}

void disableAllInterruptGates(){
    for (int i = 0; i < 256; i++)
    {
        i686_IDT_DisableGate(i);
    }
}