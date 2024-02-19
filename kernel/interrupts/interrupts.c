void enableInterrupts(){
    __asm__ __volatile__("sti");
}

void disableInterrupts(){
    __asm__ __volatile__("cli");
}