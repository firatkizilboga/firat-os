#include <stdint.h>
#include <stddef.h>
#include <pdpt.h>
typedef struct {
    uint32_t present   : 1;
    uint32_t rw        : 1;
    uint32_t user      : 1;
    uint32_t pwt       : 1;
    uint32_t pcd       : 1;
    uint32_t accessed  : 1;
    uint32_t dirty     : 1;
    uint32_t ps        : 1;
    uint32_t global    : 1;
    uint32_t available : 3;
    uint32_t addr      : 20;
} PDE;

typedef struct {
    uint32_t present   : 1;
    uint32_t rw        : 1;
    uint32_t user      : 1;
    uint32_t pwt       : 1;
    uint32_t pcd       : 1;
    uint32_t accessed  : 1;
    uint32_t dirty     : 1;
    uint32_t pat       : 1;
    uint32_t global    : 1;
    uint32_t available : 3;
    uint32_t frame     : 20;
} PTE;


static PDE create_pde() {
    PDE pde = {0};
    return pde;
}

// Function to set a PDE with specific values
static void set_pde(PDE *pde, uint32_t addr, uint8_t present, uint8_t rw, uint8_t user) {
    if (pde != NULL) {
        pde->addr = addr >> 12;  // Assume address is page aligned
        pde->present = present;
        pde->rw = rw;
        pde->user = user;
        pde->ps = 0;
    }
}

// Function to create a new PTE with default values
static PTE create_pte() {
    PTE pte = {0};
    return pte;
}
// Function to set a PTE with specific values
static void set_pte(PTE *pte, uint32_t frame, uint8_t present, uint8_t rw, uint8_t user) {
    if (pte != NULL) {
        pte->frame = frame >> 12;  // Assume frame address is page aligned
        pte->present = present;
        pte->rw = rw;
        pte->user = user;
    }
}


PDE PD[1024] __attribute__((aligned(4096))) ;
PTE firstPT[1024] __attribute__((aligned(4096))) ;

void initPaging(){
    loadPageDirectory(PD);
}

