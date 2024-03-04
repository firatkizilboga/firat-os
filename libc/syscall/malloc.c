#include <stddef.h>
#include <stdint.h>

// Assuming SYSCALL_MALLOC is the system call number for mallo
#define SYSCALL_MALLOC 0

void* malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    void* result;

    // Load the syscall number for malloc into eax, and the argument into ebx
    asm volatile (
        "movl %1, %%eax\n\t"  // Load SYSCALL_MALLOC into eax
        "movl %2, %%ebx\n\t"  // Load 'size' into ebx
        "int $0x80\n\t"       // Trigger the interrupt
        "movl %%eax, %0"      // Store the result in 'result'
        : "=r" (result)       // Output operand
        : "i" (SYSCALL_MALLOC), "r" (size)
        : "eax", "ebx"        // Clobbered registers
    );

    return result;
}