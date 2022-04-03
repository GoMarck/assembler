#pragma once

#ifndef _ASSEMBLER_MEMORY_DRAM_H_
#define _ASSEMBLER_MEMORY_DRAM_H_

#include <stdint.h>

#define MEM_SIZE 1000

uint8_t mem[MEM_SIZE];

uint64_t dram_read(void *addr);

void dram_write(void *addr, uint64_t data);

#endif