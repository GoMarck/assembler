#pragma once

#ifndef _ASSEMBLER_MEMORY_INST_H_
#define _ASSEMBLER_MEMORY_INST_H_

#include <stdint.h>

typedef enum OPERAND_TYPE_ENUM
{
    IMM,
    REG,
    MEM
} od_type_t;

// define operand.
typedef struct OPERAND_STRUNCT
{
    od_type_t type;
    int64_t imm;
    int64_t scal;
    uint64_t *reg_b; // base register
    uint64_t *reg_i; // index register
} od_t;

// define opeartion. e.g. MOV, CALL...
typedef enum OPRATION_ENUM
{
    MOV,
    ADD,
    CALL,
    RET,
    PUSH,
    POP
} op_t;

// define instruction. e.g. mov %rdx, %rdi
typedef struct INSTRUCTION_STRUCT
{
    od_t src;
    od_t dst;
    op_t op;
    char dsp[100];
} inst_t;

// init instruction handler table.
void init_handler_table();

#endif