#include <stdint.h>
#include <stddef.h>

uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag);
void setGdt(uint16_t limit, uint32_t base);
