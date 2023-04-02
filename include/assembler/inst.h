#pragma once

#ifndef ASSEMBLER_INST_H
#define ASSEMBLER_INST_H

#include <stdint.h>

typedef enum OperandType { NUL, IMM, REG, MEM } OperandType;

// define operand.
typedef struct operand_t {
  OperandType type;
  int64_t imm;
  int64_t scal;
  uint64_t *reg_b;  // base register
  uint64_t *reg_i;  // index register
} operand_t;

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
typedef struct inst_t {
  operand_t src;
  operand_t dst;
  Operation op;
  const char *code;
} inst_t;

// init instruction handler table.
void init_handler_table();

// parse instruction and effect cpu register and memory.
void parse_inst();

// Parse string into instruction set. There are two parts combine to an
// instruction: operation and operand. The forms of the operation parsed
// are shown as follow:
//
//    * mov, add, sub, call, ret, push, pop, jmp, jne
//
// And the forms of the operand parsed are shown as follow:
//
// | Type      | Form           | Operand value          | Name                |
// |-----------+----------------+------------------------+---------------------|
// | Immediate | $Imm           | Imm                    | Immediate           |
// | Register  | r_a            | R[r_a]                 | Register            |
// | Memory    | Imm            | M[Imm]                 | Absolute            |
// | Memory    | (r_a)          | M[R[r_a]]              | Indirect            |
// | Memory    | Imm(r_b)       | M[Imm+R[r_b]]          | Base + displacement |
// | Memory    | (r_b,r_i)      | M[R[r_b]+R[r_i]]       | Indexed             |
// | Memory    | Imm(r_b,r_i)   | M[Imm+R[r_b]+R[r_i]]   | Indexed             |
// | Memory    | (,r_i,s)       | M[R[r_i]*s]            | Scaled indexed      |
// | Memory    | Imm(,r_i,s)    | M[Imm+R[r_i]*s]        | Scaled indexed      |
// | Memory    | (r_b,r_i,s)    | M[R[r_b]+R[r_i]*s]     | Scaled indexed      |
// | Memory    | Imm(r_b,r_i,s) | M[Imm+R[r_b]+R[r_i]*s] | Scaled indexed      |
//
// @param[in] str The instruction string to be parsed.
// @param[out] inst The parsed instruction is output here.
void parse_instruction_str(const char *str, inst_t *inst);

void parse_operand(const char *str, operand_t *operand);

#endif  // ASSEMBLER_INST_H