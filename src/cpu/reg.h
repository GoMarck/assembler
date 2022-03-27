#pragma once

#ifndef _ASSEMBLER_CPU_REG_H_
#define _ASSEMBLER_CPU_REG_H_

#include <stdint.h>

typedef struct REG_STRUCT 
{
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t rip;
} reg_t;

reg_t reg;

#endif