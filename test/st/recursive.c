#include <stdio.h>

#include "assembler/cpu.h"
#include "assembler/dram.h"
#include "assembler/inst.h"
#include "assembler/mmu.h"
#include "test/test.h"

#define CODE_SIZE 20

// This is the assembly code translated from a simple C program:
//
// #include <stdint.h>

// uint64_t sum(uint64_t n)
// {
//     if (n == 0) {
//         return 0;
//     } else {
//         return n + sum(n - 1);
//     }
// }

// int main()
// {
//     uint64_t a = sum(3);
//     return 0;
// }