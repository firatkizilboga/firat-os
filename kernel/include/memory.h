#define BLOCK_SIZE 1024
void* allocate_chunks(int chunk_amt);
void mark_memory_range_as_free(int start, int end);
void mark_memory_range_as_used(int start, int end);
