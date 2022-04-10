#pragma once

#ifndef ASSEMBLER_CPU_REG_H
#define ASSEMBLER_CPU_REG_H

#include <stdint.h>

typedef struct Register {
  uint64_t rax;
  uint64_t rbx;
  uint64_t rcx;
  uint64_t rdx;
  uint64_t rsi;
  uint64_t rdi;
  uint64_t rbp;
  uint64_t rsp;
  uint64_t rip;
} Register;

Register reg;

#endif