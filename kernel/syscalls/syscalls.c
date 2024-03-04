#include <stdint.h>
#include <syscalls.h>
#include <memory.h>
typedef struct {
    uint32_t eax, ebx, ecx, edx;
    uint32_t esi, edi, ebp;
    uint32_t esp;
} registers_t;

void save_registers(registers_t* regs) {
    asm volatile (
        "movl %%eax, %0\n\t"
        "movl %%ebx, %1\n\t"
        "movl %%ecx, %2\n\t"
        "movl %%edx, %3\n\t"
        "movl %%esi, %4\n\t"
        "movl %%edi, %5\n\t"
        "movl %%ebp, %6\n\t"
        // "movl %%esp, %7\n\t" // Uncomment if you manually save ESP
        // Note: EIP and EFLAGS are saved automatically by pusha/popa or interrupt mechanism
        : "=m" (regs->eax), "=m" (regs->ebx), "=m" (regs->ecx), "=m" (regs->edx),
          "=m" (regs->esi), "=m" (regs->edi), "=m" (regs->ebp)
        // "=m" (regs->esp) // Uncomment if you manually save ESP
        // "m" means memory operand
        :
        : "memory" // Tells the compiler that the memory is being modified
    );
}

void restore_registers(registers_t* regs) {
    asm volatile (
        "movl %0, %%eax\n\t"
        "movl %1, %%ebx\n\t"
        "movl %2, %%ecx\n\t"
        "movl %3, %%edx\n\t"
        "movl %4, %%esi\n\t"
        "movl %5, %%edi\n\t"
        "movl %6, %%ebp\n\t"
        // "movl %7, %%esp\n\t" // Uncomment if you manually restore ESP
        // Note: EIP and EFLAGS are restored automatically by iret or similar mechanism
        :
        : "m" (regs->eax), "m" (regs->ebx), "m" (regs->ecx), "m" (regs->edx),
          "m" (regs->esi), "m" (regs->edi), "m" (regs->ebp)
        // "m" (regs->esp) // Uncomment if you manually restore ESP
        : "eax", "ebx", "ecx", "edx", "esi", "edi", "ebp" // Clobbered registers
    );
}

void syscall_handler() {
    uint32_t syscall_number;
    
    // Retrieve the syscall number from the eax register
    asm volatile ("movl %%eax, %0" : "=r"(syscall_number));

    switch (syscall_number) {
    case SYSCALL_MALLOC:
        {
            size_t size;
            void* ptr;   

            // Retrieve the size argument from the ebx register
            asm volatile ("movl %%ebx, %0" : "=r"(size));
            ptr = syscall_malloc(size);

            // Return the pointer by placing it in the eax register
            asm volatile ("movl %0, %%eax" : : "r"(ptr));
            break;
        }

    case SYSCALL_FREE:
        {
            void*ptr;
            asm volatile ("movl %%ebx, %0" : "=r"(ptr));
            syscall_free(ptr);
            break;
        }
        
    default:
        // Handle unknown syscall
        break;
    }
}

void* syscall_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    int chunk_amt = (size + sizeof(int) + BLOCK_SIZE - 1) / BLOCK_SIZE;
    return allocate_chunks(chunk_amt);
}

void syscall_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    // Get the address where the size is stored (one int before the given pointer)
    int* size_ptr = (int*)ptr - 1;
    int chunk_amt = *size_ptr;

    // Calculate the starting block index
    uintptr_t address = (uintptr_t)size_ptr;
    int start = address_to_index(address);
    mark_memory_range_as_free(start, start + chunk_amt);
}