#pragma once

#ifndef ASSEMBLER_INST_H
#define ASSEMBLER_INST_H

#include <stdint.h>

typedef enum OperandType { NUL, IMM, REG, MEM } OperandType;

// define operand.
typedef struct Operand {
  OperandType type;
  int64_t imm;
  int64_t scal;
  uint64_t *reg_b;  // base register
  uint64_t *reg_i;  // index register
} Operand;

// define opeartion. e.g. MOV, CALL...
typedef enum Operation {
  MOV_IMM_REG,
  MOV_MEM_REG,
  MOV_REG_REG,
  MOV_IMM_MEM,
  MOV_REG_MEM,
  ADD_REG_REG,
  CALL,
  RET,
  PUSH,
  POP
} Operation;

// define instruction. e.g. mov %rdx, %rdi
typedef struct Instruction {
  Operand src;
  Operand dst;
  Operation op;
  char code[100];
} Instruction;

// init instruction handler table.
void init_handler_table();

// parse instruction and effect cpu register and memory.
void parse_inst();

#endif  // ASSEMBLER_INST_H