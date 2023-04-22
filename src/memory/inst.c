#include "assembler/inst.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler/convert.h"
#include "assembler/cpu.h"
#include "assembler/dram.h"
#include "assembler/log.h"
#include "assembler/mmu.h"

#define RIP_UPDATE(cr) ((cr)->rip += INSTRUCTION_SIZE)
#define RIP_SET(cr, val) ((cr)->rip = (val))

static uint64_t get_mem_addr(operand_t *od) {
  if (od->type != MEM) {
    LOG(FATAL, "Error occured when get operand memory address, type: %d",
        od->type);
  }
  uint64_t addr = od->imm;
  if (od->reg_b != NULL) {
    addr += *od->reg_b;
  }
  if (od->reg_i != NULL) {
    addr += od->scal * (*od->reg_i);
  }
  return addr;
}

static uint64_t get_operand_val(operand_t *od) {
  uint64_t val = 0;
  if (od->type == IMM) {
    val = od->imm;
  } else if (od->type == REG) {
    val = *od->reg_b;
  } else if (od->type == MEM) {
    val = dram_read(va2pa(get_mem_addr(od)));
  } else {
    LOG(FATAL, "Error occured when get operand value, type: %d\n", od->type);
  }
  return val;
}

static void set_operand_val(operand_t *od, uint64_t val) {
  if (od->type == REG) {
    *od->reg_b = val;
  } else if (od->type == MEM) {
    dram_write(va2pa(get_mem_addr(od)), val);
  } else {
    LOG(FATAL, "Error occured when set operand value, type: %d\n", od->type);
  }
}

void mov_handler(operand_t *src, operand_t *dst) {
  set_operand_val(dst, get_operand_val(src));
  RIP_UPDATE(&core);
}

void add_handler(operand_t *src, operand_t *dst) {
  uint64_t src_val = get_operand_val(src);
  uint64_t dst_val = get_operand_val(dst);
  set_operand_val(dst, src_val + dst_val);

  // flags set
  uint64_t res_val = get_operand_val(dst);
  CLEAR_FLAGS(&core);
  CF_WR(&core, res_val < src_val);
  ZF_WR(&core, res_val == 0);
  SF_WR(&core, res_val >> 0x3f);
  OF_WR(&core, (((int64_t)src_val < 0) == ((int64_t)dst_val < 0)) &&
               (((int64_t)res_val < 0) != ((int64_t)src_val < 0)));

  RIP_UPDATE(&core);
}

void sub_handler(operand_t *src, operand_t *dst) {
  uint64_t src_val = get_operand_val(src);
  src_val = ~src_val + 1;
  uint64_t dst_val = get_operand_val(dst);
  set_operand_val(dst, src_val + dst_val);

  // flags set
  uint64_t res_val = get_operand_val(dst);
  CLEAR_FLAGS(&core);
  CF_WR(&core, res_val < src_val);
  ZF_WR(&core, res_val == 0);
  SF_WR(&core, res_val >> 0x3f);
  OF_WR(&core, (((int64_t)src_val < 0) == ((int64_t)dst_val < 0)) &&
               (((int64_t)res_val < 0) != ((int64_t)src_val < 0)));

  RIP_UPDATE(&core);
}

void call_handler(operand_t *src, operand_t *dst) {
  core.reg.rsp -= 0x8;
  dram_write(va2pa(core.reg.rsp), core.rip + INSTRUCTION_SIZE);
  RIP_SET(&core, get_operand_val(src));
}

void ret_handler(operand_t *src, operand_t *dst) {
  RIP_SET(&core, dram_read(va2pa(core.reg.rsp)));
  core.reg.rsp += 0x8;
}

void push_handler(operand_t *src, operand_t *dst) {
  core.reg.rsp -= 0x8;
  dram_write(va2pa(core.reg.rsp), get_operand_val(src));
  RIP_UPDATE(&core);
}

void pop_handler(operand_t *src, operand_t *dst) {
  set_operand_val(src, dram_read(va2pa(core.reg.rsp)));
  core.reg.rsp += 0x8;
  RIP_UPDATE(&core);
}

void jmp_handler(operand_t *src, operand_t *dst) {
  RIP_SET(&core, get_operand_val(src));
}

void jne_handler(operand_t *src, operand_t *dst) {
  if (!ZF_RD(&core)) {
    RIP_SET(&core, get_operand_val(src));
  } else {
    RIP_UPDATE(&core);
  }
}

// leave equals to:
// movq %rbp, %rsp Set stack pointer to beginning of frame
// popq %rbp       Restore saved %rbp and set stack ptr to
//                 end of caller’s frame
void leave_handler(operand_t *src, operand_t *dst) {
  operand_t rbp_od;
  rbp_od.reg_b = &core.reg.rbp;
  rbp_od.type = REG;

  operand_t rsp_od;
  rsp_od.reg_b = &core.reg.rsp;
  rsp_od.type = REG;

  set_operand_val(&rsp_od, get_operand_val(&rbp_od));
  set_operand_val(&rbp_od, dram_read(va2pa(core.reg.rsp)));
  core.reg.rsp += 0x8;
  RIP_UPDATE(&core);
}

void cmp_handler(operand_t *src, operand_t *dst) {
  uint64_t src_val = get_operand_val(src);
  src_val = ~src_val + 1;
  uint64_t dst_val = get_operand_val(dst);
  uint64_t res_val = src_val + dst_val;

  // flags set
  CLEAR_FLAGS(&core);
  CF_WR(&core, res_val < src_val);
  ZF_WR(&core, res_val == 0);
  SF_WR(&core, res_val >> 0x3f);
  OF_WR(&core, (((int64_t)src_val < 0) == ((int64_t)dst_val < 0)) &&
               (((int64_t)res_val < 0) != ((int64_t)src_val < 0)));
  RIP_UPDATE(&core);
}

// init instruction handler table.
void init_handler_table() {
  handler_table[MOV] = &mov_handler;
  handler_table[ADD] = &add_handler;
  handler_table[SUB] = &sub_handler;
  handler_table[CALL] = &call_handler;
  handler_table[RET] = &ret_handler;
  handler_table[PUSH] = &push_handler;
  handler_table[POP] = &pop_handler;
  handler_table[JMP] = &jmp_handler;
  handler_table[JNE] = &jne_handler;
  handler_table[LEAVE] = &leave_handler;
  handler_table[CMP] = &cmp_handler;
}

void parse_instruction() {
  char inst_str[INSTRUCTION_SIZE];
  dram_read_instruction(va2pa(core.rip), inst_str, INSTRUCTION_SIZE);
  inst_t inst;
  parse_instruction_str(inst_str, &core, &inst);
  handler_t handler = handler_table[inst.op];
  handler(&inst.src, &inst.dst);
  LOG(VERBOSE, "%s", inst.code);
}

void parse_instruction_str(const char *str, core_t *cr, inst_t *inst) {
  char op[64];
  uint32_t op_len = 0;
  char src[64];
  uint32_t src_len = 0;
  char dst[64];
  uint32_t dst_len = 0;

  uint8_t bracket_cnt = 0;
  uint32_t state = 0;
  size_t size = strlen(str);

  for (size_t i = 0; i < size; ++i) {
    char c = str[i];
    if (c == '(' || c == ')') {
      ++bracket_cnt;
    }

    if (state == 0) {
      if (c == ' ') {
        continue;
      }
      state = 1;
      op[op_len++] = c;
    } else if (state == 1) {
      if (c == ' ') {
        state = 2;
        continue;
      } else {
        op[op_len++] = c;
      }
    } else if (state == 2) {
      if (c == ' ') {
        continue;
      } else {
        state = 3;
        src[src_len++] = c;
      }
    } else if (state == 3) {
      if (c == ',' && (bracket_cnt == 0 || bracket_cnt == 2)) {
        state = 4;
      } else {
        src[src_len++] = c;
      }
    } else if (state == 4) {
      if (c != ' ') {
        state = 5;
        dst[dst_len++] = c;
      }
    } else if (state == 5) {
      dst[dst_len++] = c;
    }
  }

  op[op_len] = '\0';
  src[src_len] = '\0';
  dst[dst_len] = '\0';

  parse_operation(op, &inst->op);
  parse_operand(src, cr, &inst->src);
  parse_operand(dst, cr, &inst->dst);
  inst->code = str;
}

void parse_operation(const char *str, op_t *op) {
  if (strcmp(str, "mov") == 0 || strcmp(str, "movq") == 0) {
    *op = MOV;
  } else if (strcmp(str, "add") == 0 || strcmp(str, "addq") == 0) {
    *op = ADD;
  } else if (strcmp(str, "sub") == 0 || strcmp(str, "subq") == 0) {
    *op = SUB;
  } else if (strcmp(str, "call") == 0 || strcmp(str, "callq") == 0) {
    *op = CALL;
  } else if (strcmp(str, "ret") == 0 || strcmp(str, "retq") == 0) {
    *op = RET;
  } else if (strcmp(str, "push") == 0 || strcmp(str, "pushq") == 0) {
    *op = PUSH;
  } else if (strcmp(str, "pop") == 0 || strcmp(str, "popq") == 0) {
    *op = POP;
  } else if (strcmp(str, "jmp") == 0) {
    *op = JMP;
  } else if (strcmp(str, "jne") == 0) {
    *op = JNE;
  } else if (strcmp(str, "leave") == 0 || strcmp(str, "leaveq") == 0) {
    *op = LEAVE;
  } else if (strcmp(str, "cmpq") == 0) {
    *op = CMP;
  } else {
    LOG(FATAL, "Error operation: [%s]", str);
  }
}

// TODO: very low performance, need optimize.
static const char *reg_name_table[64] = {
    "%rax", "%eax",  "%ax",   "%al",   "%rbx", "%ebx",  "%bx",   "%bl",
    "%rcx", "%ecx",  "%cx",   "%cl",   "%rdx", "%edx",  "%dx",   "%dl",
    "%rsi", "%esi",  "%si",   "%sil",  "%rdi", "%edi",  "%di",   "%dil",
    "%rbp", "%ebp",  "%bp",   "%bpl",  "%rsp", "%esp",  "%sp",   "%spl",
    "%r8",  "%r8d",  "%r8w",  "%r8b",  "%r9",  "%r9d",  "%r9w",  "%r9b",
    "%r10", "%r1d",  "%r10w", "%r10b", "%r11", "%r11d", "%r11w", "%r11b",
    "%r12", "%r12d", "%r12w", "%r12b", "%r13", "%r13d", "%r13w", "%r13b",
    "%r14", "%r14d", "%r14w", "%r14b", "%r15", "%r15d", "%r15w", "%r15b"};

static uint64_t *reflect_register(const char *str, core_t *cr) {
  uint64_t *reg_addr_table[64] = {
      (uint64_t *)&cr->reg.rax,  (uint64_t *)&cr->reg.eax,
      (uint64_t *)&cr->reg.ax,   (uint64_t *)&cr->reg.al,
      (uint64_t *)&cr->reg.rbx,  (uint64_t *)&cr->reg.ebx,
      (uint64_t *)&cr->reg.bx,   (uint64_t *)&cr->reg.bl,
      (uint64_t *)&cr->reg.rcx,  (uint64_t *)&cr->reg.ecx,
      (uint64_t *)&cr->reg.cx,   (uint64_t *)&cr->reg.cl,
      (uint64_t *)&cr->reg.rdx,  (uint64_t *)&cr->reg.edx,
      (uint64_t *)&cr->reg.dx,   (uint64_t *)&cr->reg.dl,
      (uint64_t *)&cr->reg.rsi,  (uint64_t *)&cr->reg.esi,
      (uint64_t *)&cr->reg.si,   (uint64_t *)&cr->reg.sil,
      (uint64_t *)&cr->reg.rdi,  (uint64_t *)&cr->reg.edi,
      (uint64_t *)&cr->reg.di,   (uint64_t *)&cr->reg.dil,
      (uint64_t *)&cr->reg.rbp,  (uint64_t *)&cr->reg.ebp,
      (uint64_t *)&cr->reg.bp,   (uint64_t *)&cr->reg.bpl,
      (uint64_t *)&cr->reg.rsp,  (uint64_t *)&cr->reg.esp,
      (uint64_t *)&cr->reg.sp,   (uint64_t *)&cr->reg.spl,
      (uint64_t *)&cr->reg.r8,   (uint64_t *)&cr->reg.r8d,
      (uint64_t *)&cr->reg.r8w,  (uint64_t *)&cr->reg.r8b,
      (uint64_t *)&cr->reg.r9,   (uint64_t *)&cr->reg.r9d,
      (uint64_t *)&cr->reg.r9w,  (uint64_t *)&cr->reg.r9b,
      (uint64_t *)&cr->reg.r10,  (uint64_t *)&cr->reg.r10d,
      (uint64_t *)&cr->reg.r10w, (uint64_t *)&cr->reg.r10b,
      (uint64_t *)&cr->reg.r11,  (uint64_t *)&cr->reg.r11d,
      (uint64_t *)&cr->reg.r11w, (uint64_t *)&cr->reg.r11b,
      (uint64_t *)&cr->reg.r12,  (uint64_t *)&cr->reg.r12d,
      (uint64_t *)&cr->reg.r12w, (uint64_t *)&cr->reg.r12b,
      (uint64_t *)&cr->reg.r13,  (uint64_t *)&cr->reg.r13d,
      (uint64_t *)&cr->reg.r13w, (uint64_t *)&cr->reg.r13b,
      (uint64_t *)&cr->reg.r14,  (uint64_t *)&cr->reg.r14d,
      (uint64_t *)&cr->reg.r14w, (uint64_t *)&cr->reg.r14b,
      (uint64_t *)&cr->reg.r15,  (uint64_t *)&cr->reg.r15d,
      (uint64_t *)&cr->reg.r15w, (uint64_t *)&cr->reg.r15b};
  for (size_t i = 0; i < 64; ++i) {
    if (strcmp(str, reg_name_table[i]) == 0) {
      return reg_addr_table[i];
    }
  }
  LOG(FATAL, "Parse register string failed: [%s]", str);
}

void parse_operand(const char *str, core_t *cr, operand_t *operand) {
  // init
  operand->imm = 0;
  operand->reg_b = NULL;
  operand->reg_i = NULL;
  operand->scal = 1;
  operand->type = NUL;

  size_t size = strlen(str);
  if (size == 0) {
    return;
  }

  if (str[0] == '$') {
    operand->type = IMM;
    operand->imm = str2uint64_range(str, 1, size);
  } else if (str[0] == '%') {
    operand->type = REG;
    operand->reg_b = reflect_register(str, cr);
  } else {
    operand->type = MEM;
    char imm[64];
    uint32_t imm_len = 0;
    char reg_b[64];
    uint32_t reg_b_len = 0;
    char reg_i[64];
    uint32_t reg_i_len = 0;
    char scal[64];
    uint32_t scal_len = 0;

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
        break;
      } else {
        LOG(FATAL, "Parse operand string and error happen: [%s]", str);
      }
    }

    if (imm_len > 0) {
      imm[imm_len] = '\0';
      operand->imm = str2uint64(imm);
    }

    if (reg_b_len > 0) {
      reg_b[reg_b_len] = '\0';
      operand->reg_b = reflect_register(reg_b, cr);
    }

    if (reg_i_len > 0) {
      reg_i[reg_i_len] = '\0';
      operand->reg_i = reflect_register(reg_i, cr);
    }

    if (scal_len > 0) {
      scal[scal_len] = '\0';
      operand->scal = str2uint64(scal);
      if (operand->scal != (int64_t)1 && operand->scal != (int64_t)2 &&
          operand->scal != (int64_t)4 && operand->scal != (int64_t)8) {
        LOG(FATAL, "Error scal in [%s], scal [%ld]", str, operand->scal);
      }
    }
  }
}