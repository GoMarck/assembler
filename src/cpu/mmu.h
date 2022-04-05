// memory manager unit
#pragma once

#ifndef _ASSEMBLER_CPU_MMU_H_
#define _ASSEMBLER_CPU_MMU_H_

#include <stdint.h>

uint64_t va2pa(uint64_t vaddr);

#endif