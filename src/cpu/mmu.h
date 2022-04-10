// memory manager unit
#pragma once

#ifndef ASSEMBLER_CPU_MMU_H
#define ASSEMBLER_CPU_MMU_H

#include <stdint.h>

uint64_t va2pa(uint64_t vaddr);

#endif