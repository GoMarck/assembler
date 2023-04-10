#pragma once

#ifndef ASSEMBLER_DRAM_H
#define ASSEMBLER_DRAM_H

#include <stddef.h>
#include <stdint.h>

#define MEM_SIZE (1 << 20)

uint8_t mem[MEM_SIZE];

uint64_t dram_read(uint64_t addr);

void dram_write(uint64_t addr, uint64_t data);

void dram_write_instruction(uint64_t addr, const char *str, size_t size);

void dram_read_instruction(uint64_t addr, const char *str, size_t size);

#endif  // ASSEMBLER_DRAM_H