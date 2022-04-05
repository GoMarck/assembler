#include "memory/dram.h"

#include <string.h>

#define SRAM_CACHE 0

uint64_t dram_read(uint64_t addr)
{
    uint64_t data;
    if (SRAM_CACHE) {
        return 0x0;
    }
    memcpy(&data, mem + addr, sizeof(data));
    return data;
}

void dram_write(uint64_t addr, uint64_t data)
{
    if (SRAM_CACHE) {
        return;
    }
    memcpy(mem + addr, &data, sizeof(data));
}