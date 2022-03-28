#include "cpu/mmu.h"

#include "memory/dram.h"

void *va2pa(void *vaddr)
{
    return (void *)((uint64_t)vaddr % MEM_SIZE);
}