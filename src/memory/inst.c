#include "assembler/inst.h"

#include <stdio.h>

#include "assembler/mmu.h"
#include "assembler/cpu.h"
#include "assembler/dram.h"

#define OP_SIZE 11

void parse_inst_str(const char *str, Instruction *inst) {
  
}

// static void parse_operand_str(const char *str, Operand *operand) {}

// static void parse_operation_str(const char *str, Operation *op) {}

typedef void (*handler_t)(uint64_t, uint64_t);

handler_t handler_table[OP_SIZE];

static void mov_imm_reg_handler(uint64_t src, uint64_t dst);
static void mov_mem_reg_handler(uint64_t src, uint64_t dst);
static void mov_reg_reg_handler(uint64_t src, uint64_t dst);
static void mov_imm_mem_handler(uint64_t src, uint64_t dst);
static void mov_reg_mem_handler(uint64_t src, uint64_t dst);
static void add_reg_reg_handler(uint64_t src, uint64_t dst);
static void call_handler(uint64_t src, uint64_t dst);
static void ret_handler(uint64_t src, uint64_t dst);
static void push_handler(uint64_t src, uint64_t dst);
static void pop_handler(uint64_t src, uint64_t dst);
static uint64_t decode_od(Operand *od);

// init instruction handler table.
void init_handler_table() {
  handler_table[MOV_IMM_REG] = &mov_imm_reg_handler;
  handler_table[MOV_MEM_REG] = &mov_mem_reg_handler;
  handler_table[MOV_REG_REG] = &mov_reg_reg_handler;
  handler_table[MOV_IMM_MEM] = &mov_imm_mem_handler;
  handler_table[MOV_REG_MEM] = &mov_reg_mem_handler;
  handler_table[ADD_REG_REG] = &add_reg_reg_handler;
  handler_table[CALL] = &call_handler;
  handler_table[RET] = &ret_handler;
  handler_table[PUSH] = &push_handler;
  handler_table[POP] = &pop_handler;
}

void parse_inst() {
  Instruction *inst = (Instruction *)core.rip;
  uint64_t src = decode_od(&inst->src);
  uint64_t dst = decode_od(&inst->dst);
  handler_t handler = handler_table[inst->op];
  handler(src, dst);
  printf("%s\n", inst->code);
}

static uint64_t decode_od(Operand *od) {
  uint64_t val = 0;
  if (od->type == IMM) {
    val = od->imm;
  } else if (od->type == REG) {
    val = (uint64_t)od->reg_b;
  } else {
    val += od->imm;
    if (od->reg_b != NULL) {
      val += *od->reg_b;
    }
    if (od->reg_i != NULL) {
      val += od->scal * *od->reg_i;
    }
  }
  return val;
}

// mov instruction implementation
static void mov_imm_reg_handler(uint64_t src, uint64_t dst) {
  *(uint64_t *)dst = src;
  core.rip += sizeof(Instruction);
}

// mov instruction implementation
static void mov_mem_reg_handler(uint64_t src, uint64_t dst) {
  *(uint64_t *)dst = dram_read(va2pa(src));
  core.rip += sizeof(Instruction);
}

// mov instruction implementation
static void mov_reg_reg_handler(uint64_t src, uint64_t dst) {
  *(uint64_t *)dst = *(uint64_t *)src;
  core.rip += sizeof(Instruction);
}

// mov instruction implementation
static void mov_imm_mem_handler(uint64_t src, uint64_t dst) {
  dram_write(va2pa(dst), src);
  core.rip += sizeof(Instruction);
}

// mov instruction implementation
static void mov_reg_mem_handler(uint64_t src, uint64_t dst) {
  dram_write(va2pa(dst), *(uint64_t *)src);
  core.rip += sizeof(Instruction);
}

// add instruction implementation
static void add_reg_reg_handler(uint64_t src, uint64_t dst) {
  *(uint64_t *)dst += *(uint64_t *)src;
  core.rip += sizeof(Instruction);
}

// call instruction implementation
static void call_handler(uint64_t src, uint64_t dst) {
  core.reg.rsp -= 0x8;
  dram_write(va2pa(core.reg.rsp), core.rip + sizeof(Instruction));
  core.rip = src;
}

// ret instruction implementation
static void ret_handler(uint64_t src, uint64_t dst) {
  core.rip = dram_read(va2pa(core.reg.rsp));
  core.reg.rsp += 0x8;
}

// call instruction implementation
static void push_handler(uint64_t src, uint64_t dst) {
  core.reg.rsp -= 0x8;
  dram_write(va2pa(core.reg.rsp), *(uint64_t *)src);
  core.rip += sizeof(Instruction);
}

// ret instruction implementation
static void pop_handler(uint64_t src, uint64_t dst) {
  *(uint64_t *)src = dram_read(va2pa(core.reg.rsp));
  core.reg.rsp += 0x8;
  core.rip += sizeof(Instruction);
}