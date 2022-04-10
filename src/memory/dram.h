#pragma once

#ifndef ASSEMBLER_MEMORY_DRAM_H
#define ASSEMBLER_MEMORY_DRAM_H

#include <stdint.h>

#define MEM_SIZE 1000

uint8_t mem[MEM_SIZE];

uint64_t dram_read(uint64_t addr);

void dram_write(uint64_t addr, uint64_t data);

#endif