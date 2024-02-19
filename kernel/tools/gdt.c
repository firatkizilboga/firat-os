#include <stdint.h> 
#include <stddef.h>


struct GDT
{
    uint32_t base;        // The base address of the segment
    uint32_t limit;       // The limit of the segment
    uint8_t access_byte;  // Access rights and properties
    uint8_t flags;        // Flags
};

void encodeGdtEntry(uint8_t *target, struct GDT source)
{
    // Check the limit to make sure that it can be encoded
    if (source.limit > 0xFFFFF) {kerror("GDT cannot encode limits larger than 0xFFFFF");}
 
    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;
 
    // Encode the base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;
 
    // Encode the access byte
    target[5] = source.access_byte;
 
    // Encode the flags
    target[6] |= (source.flags << 4);
}

void disableInterrupts()
{
    __asm__ __volatile__("cli");
}

void enableInterrupts()
{
    __asm__ __volatile__("sti");
}

int is_protected_mode() {
    uint32_t cr0;
    __asm__("mov %%cr0, %0" : "=r"(cr0)); // Move the value of CR0 into the variable cr0
    return cr0 & 0x1; // Check the PE bit (bit 0) of CR0
}
