#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "terminal.h"
#include "video.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <keypress.h>
#include <editor.h>
#include <timer.h>
#include <pdpt.h>
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21

void disable_timer_interrupt() {
  unsigned char mask;

  // Read the current mask
  asm volatile("inb %1, %0" : "=a"(mask) : "Nd"(PIC1_DATA));

  // Set the mask for IRQ0
  mask |= 0x01;

  // Write the new mask
  outb(PIC1_DATA, mask);
}


void handlePageFault(){
  disableInterrupts();
  Cursor* c = getCurrentCursor();
  VGATextFrame* f = getCurrentFrame();
  update_cursor(c,0,2);
  printf("Page Fault!");
  __asm__ volatile ("cli; hlt"); // Completely hangs the computer
  enableInterrupts();
}

void kernel_main(void) {
    maskIRQ(0);
    disableInterrupts();
    i686_GDT_Initialize();
    i686_IDT_Initialize();
    initVideo();
    initKeyboard();
	  initTimer();

    i686_IDT_EnableGate(14);
    i686_IDT_SetGate(14, handlePageFault, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);

    i686_IDT_EnableGate(8);
    i686_IDT_SetGate(8, handlePageFault, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    
    initPaging();
    enableInterrupts();
    terminal();
}
