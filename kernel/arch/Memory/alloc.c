#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#define TOTAL_MEMORY_BLOCKS 120 * 1024
#define BLOCK_SIZE 1024

static int bitmap[TOTAL_MEMORY_BLOCKS] = {0};

extern char kernel_start[]; // Defined by linker script
extern char kernel_end[];   // Defined by linker script

static size_t address_to_index(uintptr_t address) {
    return (address - (uintptr_t)kernel_start) / BLOCK_SIZE;
}

static mark_memory_range_as_used(int start, int end){
    for (size_t i = start; i <= end; i++)
    {
        bitmap[i] = 1;
    }
};

void mark_kernel_memory() {
    uintptr_t start = (uintptr_t)kernel_start;
    uintptr_t end = (uintptr_t)kernel_end;

    // Assuming a function that marks a range of memory as used
    mark_memory_range_as_used(start, end);
}



static mark_memory_range_as_free(int start, int end){
    for (size_t i = start; i <= end; i++)
    {
        bitmap[i] = 0;
    }
};

static uintptr_t index_to_address(size_t index) {
    return (uintptr_t)kernel_start + index * BLOCK_SIZE;
}

static void* allocate_chunks(int chunk_amt) {
    for (int end = TOTAL_MEMORY_BLOCKS - 1; end >= chunk_amt; end--) {
        bool isFree = true;
        int start = end - chunk_amt + 1;
        for (int i = start; i <= end; i++) {
            if (bitmap[i]) {
                isFree = false;
                break;
            }
        }
        if (isFree) {
            mark_memory_range_as_used(start, end);
            int* ptr = (int*)index_to_address(start);
            *ptr = chunk_amt;  // Store the number of allocated blocks at the start
            return (void*)(ptr+1);  // Return address after the size storage
        }
    }
    return NULL; // No suitable range found
}

void* malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    int chunk_amt = (size + sizeof(int) + BLOCK_SIZE - 1) / BLOCK_SIZE;
    return allocate_chunks(chunk_amt);
}

void free(void* ptr) {
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