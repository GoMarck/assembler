#pragma once

#ifndef ASSEMBLER_INST_H
#define ASSEMBLER_INST_H

#include <stdint.h>

#include "assembler/cpu.h"

#define OP_SIZE 11
#define INSTRUCTION_SIZE 64

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
typedef enum op_t {
  MOV,
  ADD,
  SUB,
  CALL,
  RET,
  PUSH,
  POP,
  JMP,
  JNE,
  LEAVE,
  CMP
} op_t;

// define instruction. e.g. mov %rdx, %rdi
typedef struct inst_t {
  operand_t src;
  operand_t dst;
  op_t op;
  const char *code;
} inst_t;

typedef void (*handler_t)(operand_t *, operand_t *);
handler_t handler_table[OP_SIZE];

// Init instruction handler table.
void init_handler_table();

// Parse instruction and effect cpu register and memory.
void parse_instruction();

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
// @param[in] cr The core instruction belongs to.
// @param[out] inst The parsed instruction is output here.
void parse_instruction_str(const char *str, core_t *cr, inst_t *inst);

// Parse operation string like mov, add, sub and so on, if parse failed, sigabrt raised.
// @param[in] str Operation string pointer.
// @param[out] op The parsed result would be stored here.
void parse_operation(const char *str, op_t *op);

// Parse operand string like %rax, $0x1234, (%rax, %rdi, 4) and so on, if parse failed, sigabrt raised.
// @param[in] str Operand string pointer.
// @param[in] cr The cpu core.
// @param[out] operand The parsed result would be stored here.
void parse_operand(const char *str, core_t *cr, operand_t *operand);

// Private zone, just for test purpose.
void mov_hander(operand_t *src, operand_t *dst);
void add_handler(operand_t *src, operand_t *dst);
void call_handler(operand_t *src, operand_t *dst);
void ret_handler(operand_t *src, operand_t *dst);
void push_handler(operand_t *src, operand_t *dst);
void pop_handler(operand_t *src, operand_t *dst);

#endif  // ASSEMBLER_INST_H