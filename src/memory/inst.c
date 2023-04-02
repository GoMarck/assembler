#include "assembler/inst.h"

#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler/convert.h"
#include "assembler/cpu.h"
#include "assembler/dram.h"
#include "assembler/mmu.h"

#define OP_SIZE 11

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
static uint64_t decode_od(operand_t *od);

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
  inst_t *inst = (inst_t *)core.rip;
  uint64_t src = decode_od(&inst->src);
  uint64_t dst = decode_od(&inst->dst);
  handler_t handler = handler_table[inst->op];
  handler(src, dst);
  printf("%s\n", inst->code);
}

static uint64_t decode_od(operand_t *od) {
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
  core.rip += sizeof(inst_t);
}

// mov instruction implementation
static void mov_mem_reg_handler(uint64_t src, uint64_t dst) {
  *(uint64_t *)dst = dram_read(va2pa(src));
  core.rip += sizeof(inst_t);
}

// mov instruction implementation
static void mov_reg_reg_handler(uint64_t src, uint64_t dst) {
  *(uint64_t *)dst = *(uint64_t *)src;
  core.rip += sizeof(inst_t);
}

// mov instruction implementation
static void mov_imm_mem_handler(uint64_t src, uint64_t dst) {
  dram_write(va2pa(dst), src);
  core.rip += sizeof(inst_t);
}

// mov instruction implementation
static void mov_reg_mem_handler(uint64_t src, uint64_t dst) {
  dram_write(va2pa(dst), *(uint64_t *)src);
  core.rip += sizeof(inst_t);
}

// add instruction implementation
static void add_reg_reg_handler(uint64_t src, uint64_t dst) {
  *(uint64_t *)dst += *(uint64_t *)src;
  core.rip += sizeof(inst_t);
}

// call instruction implementation
static void call_handler(uint64_t src, uint64_t dst) {
  core.reg.rsp -= 0x8;
  dram_write(va2pa(core.reg.rsp), core.rip + sizeof(inst_t));
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
  core.rip += sizeof(inst_t);
}

// ret instruction implementation
static void pop_handler(uint64_t src, uint64_t dst) {
  *(uint64_t *)src = dram_read(va2pa(core.reg.rsp));
  core.reg.rsp += 0x8;
  core.rip += sizeof(inst_t);
}

void parse_instruction_str(const char *str, inst_t *inst) {

}

// TODO: very low performance, need optimize.
static const char *reg_name_table[64] = {
  "%rax", "%eax",  "%ax",   "%al",
  "%rbx", "%ebx",  "%bx",   "%bl",
  "%rcx", "%ecx",  "%cx",   "%cl",
  "%rdx", "%edx",  "%dx",   "%dl",
  "%rsi", "%esi",  "%si",   "%sil",
  "%rdi", "%edi",  "%di",   "%dil",
  "%rbp", "%ebp",  "%bp",   "%bpl",
  "%rsp", "%esp",  "%sp",   "%spl",
  "%r8",  "%r8d",  "%r8w",  "%r8b",
  "%r9",  "%r9d",  "%r9w",  "%r9b",
  "%r10", "%r1d",  "%r10w", "%r10b",
  "%r11", "%r11d", "%r11w", "%r11b",
  "%r12", "%r12d", "%r12w", "%r12b",
  "%r13", "%r13d", "%r13w", "%r13b",
  "%r14", "%r14d", "%r14w", "%r14b",
  "%r15", "%r15d", "%r15w", "%r15b"
};

static uint64_t *reflect_register(const char *str, core_t *cr) {
  uint64_t *reg_addr_table[64] = {
    (uint64_t *)&cr->reg.rax, (uint64_t *)&cr->reg.eax,  (uint64_t *)&cr->reg.ax,   (uint64_t *)&cr->reg.al,
    (uint64_t *)&cr->reg.rbx, (uint64_t *)&cr->reg.ebx,  (uint64_t *)&cr->reg.bx,   (uint64_t *)&cr->reg.bl,
    (uint64_t *)&cr->reg.rcx, (uint64_t *)&cr->reg.ecx,  (uint64_t *)&cr->reg.cx,   (uint64_t *)&cr->reg.cl,
    (uint64_t *)&cr->reg.rdx, (uint64_t *)&cr->reg.edx,  (uint64_t *)&cr->reg.dx,   (uint64_t *)&cr->reg.dl,
    (uint64_t *)&cr->reg.rsi, (uint64_t *)&cr->reg.esi,  (uint64_t *)&cr->reg.si,   (uint64_t *)&cr->reg.sil,
    (uint64_t *)&cr->reg.rdi, (uint64_t *)&cr->reg.edi,  (uint64_t *)&cr->reg.di,   (uint64_t *)&cr->reg.dil,
    (uint64_t *)&cr->reg.rbp, (uint64_t *)&cr->reg.ebp,  (uint64_t *)&cr->reg.bp,   (uint64_t *)&cr->reg.bpl,
    (uint64_t *)&cr->reg.rsp, (uint64_t *)&cr->reg.esp,  (uint64_t *)&cr->reg.sp,   (uint64_t *)&cr->reg.spl,
    (uint64_t *)&cr->reg.r8,  (uint64_t *)&cr->reg.r8d,  (uint64_t *)&cr->reg.r8w,  (uint64_t *)&cr->reg.r8b,
    (uint64_t *)&cr->reg.r9,  (uint64_t *)&cr->reg.r9d,  (uint64_t *)&cr->reg.r9w,  (uint64_t *)&cr->reg.r9b,
    (uint64_t *)&cr->reg.r10, (uint64_t *)&cr->reg.r10d, (uint64_t *)&cr->reg.r10w, (uint64_t *)&cr->reg.r10b,
    (uint64_t *)&cr->reg.r11, (uint64_t *)&cr->reg.r11d, (uint64_t *)&cr->reg.r11w, (uint64_t *)&cr->reg.r11b,
    (uint64_t *)&cr->reg.r12, (uint64_t *)&cr->reg.r12d, (uint64_t *)&cr->reg.r12w, (uint64_t *)&cr->reg.r12b,
    (uint64_t *)&cr->reg.r13, (uint64_t *)&cr->reg.r13d, (uint64_t *)&cr->reg.r13w, (uint64_t *)&cr->reg.r13b,
    (uint64_t *)&cr->reg.r14, (uint64_t *)&cr->reg.r14d, (uint64_t *)&cr->reg.r14w, (uint64_t *)&cr->reg.r14b,
    (uint64_t *)&cr->reg.r15, (uint64_t *)&cr->reg.r15d, (uint64_t *)&cr->reg.r15w, (uint64_t *)&cr->reg.r15b
  };
  for (size_t i = 0; i < sizeof(reg_name_table); ++i) {
    if (strcmp(str, reg_name_table[i]) == 0) {
      return reg_addr_table[i];
    }
  }
  printf("parse register failed: %s\n", str);
  abort();
}

void parse_operand(const char *str, operand_t *operand) {
  // init
  operand->imm = 0;
  operand->reg_b = NULL;
  operand->reg_i = NULL;
  operand->scal = 1;
  operand->type = NUL;

  size_t size = strlen(str);
  if (size == 0) {
    printf("operand is empty");
    abort();
  }

  if (str[0] == '$') {
    operand->type = IMM;
    operand->imm = str2uint64_range(str, 1, size);
  } else if (str[0] == '%') {
    operand->type = REG;
    operand->reg_b = reflect_register(str, &core);
  } else {
    operand->type = MEM;
    char imm[64];
    uint32_t imm_len = 0;
    char reg_b[64];
    uint32_t reg_b_len = 0;
    char reg_i[64];
    uint32_t reg_i_len = 0;
    char scal[64];
    uint32_t scal_len = 64;

    uint8_t bracket_cnt = 0;
    uint8_t comma_cnt = 0;
    for (size_t i = 0; i < size; ++i) {
      if (str[i] == '(' || str[i] == ')') {
        ++bracket_cnt;
        continue;
      } else if (str[i] == ',') {
        ++comma_cnt;
        continue;
      }

      if (bracket_cnt == 0 && comma_cnt == 0) {
        imm[imm_len++] = str[i];
      } else if (bracket_cnt == 1 && comma_cnt == 0) {
        reg_b[reg_b_len++] = str[i];
      } else if (bracket_cnt == 1 && comma_cnt == 1) {
        reg_i[reg_i_len++] = str[i];
      } else if (bracket_cnt == 1 && comma_cnt == 2) {
        scal[scal_len++] = str[i];
      } else if (bracket_cnt == 2 && comma_cnt == 2) {
        if (imm_len > 0) {
          imm[imm_len] = '\0';
          operand->imm = str2uint64(imm);
        } 

        if (reg_b_len > 0) {
          reg_b[reg_b_len] = '\0';
          operand->reg_b = reflect_register(reg_b, &core);
        }
        
        if (reg_i_len > 0) {
          reg_i[reg_i_len] = '\0';
          operand->reg_i = reflect_register(reg_i, &core);
        }
        
        if (scal_len > 0) {
          scal[scal_len] = '\0';
          operand->scal = str2uint64(scal);
          if (operand->scal != (int64_t)1 && operand->scal != (int64_t)2 &&
              operand->scal != (int64_t)4 && operand->scal != (int64_t)8) {
            printf("error scal in %s, scal %ld\n", str, operand->scal);
            abort();
          }
        }
      } else {
        printf("parse operand error happen: %s\n", str);
        abort();
      }
    }
  }
}