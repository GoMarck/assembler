#include "assembler/mmu.h"

#include "assembler/dram.h"

uint64_t va2pa(uint64_t vaddr) { return vaddr % MEM_SIZE; }