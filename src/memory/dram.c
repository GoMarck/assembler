#include "memory/dram.h"

#include <string.h>

#define SRAM_CACHE 0

uint64_t dram_read(void *addr)
{
    uint64_t data;
    if (SRAM_CACHE) {
        return 0x0;
    }
    memcpy(&data, addr, sizeof(data));
    return data;
}

void dram_write(void *addr, uint64_t data)
{
    if (SRAM_CACHE) {
        return;
    }
    memcpy(addr, &data, sizeof(data));
}