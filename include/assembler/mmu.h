// memory manager unit
#pragma once

#ifndef ASSEMBLER_MMU_H
#define ASSEMBLER_MMU_H

#include <stdint.h>

// Convert virtual memory address to phyical memory address.
// @param vaddr Virtual memory address.
// @return Phyical memory address.
uint64_t va2pa(uint64_t vaddr);

#endif  // ASSEMBLER_MMU_H