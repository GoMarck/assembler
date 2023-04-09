#include "assembler/inst.h"

#include <stdio.h>

#include "assembler/cpu.h"
#include "test/test.h"

TEST_BEGIN(test_parse_operand_immediate_number) {
  size_t imm_size = 14;

  char *imm_operand_list[] = {
    "$0",
    "$-0",
    "$0x0",
    "$1234",
    "$0x1234",
    "$0xabcd",
    "$-0xabcd",
    "$-1234",
    "$2147483647",
    "$-2147483648",
    "$0x8000000000000000",
    "$0xffffffffffffffff",
    "$0177",
    "$-0177"
  };

  uint64_t cmp_list[] = {
    0, 0, 0, 1234, 0x1234, 0xabcd, -0xabcd,
    -1234, 2147483647, -2147483648,
    0x8000000000000000, 0xffffffffffffffff,
    0177, -0177
  };

  for (size_t i = 0; i < imm_size; ++i) {
    operand_t op;
    parse_operand(imm_operand_list[i], &core, &op);
    EXPECT_U32_EQ(op.type, IMM);
    EXPECT_U64_EQ(op.imm, cmp_list[i]);
  }
}
TEST_END

TEST_BEGIN(test_parse_operand_register) {
  size_t reg_size = 64;

  char *reg_operand_list[] = {
      "%rax",  "%eax",  "%ax",   "%al",   "%rbx",  "%ebx",  "%bx",
      "%bl",   "%rcx",  "%ecx",  "%cx",   "%cl",   "%rdx",  "%edx",
      "%dx",   "%dl",   "%rsi",  "%esi",  "%si",   "%sil",  "%rdi",
      "%edi",  "%di",   "%dil",  "%rbp",  "%ebp",  "%bp",   "%bpl",
      "%rsp",  "%esp",  "%sp",   "%spl",  "%r8",   "%r8d",  "%r8w",
      "%r8b",  "%r9",   "%r9d",  "%r9w",  "%r9b",  "%r10",  "%r1d",
      "%r10w", "%r10b", "%r11",  "%r11d", "%r11w", "%r11b", "%r12",
      "%r12d", "%r12w", "%r12b", "%r13",  "%r13d", "%r13w", "%r13b",
      "%r14",  "%r14d", "%r14w", "%r14b", "%r15",  "%r15d", "%r15w",
      "%r15b"
  };

  uint64_t *cmp_reg_addr_list[] = {
      (uint64_t *)&core.reg.rax,  (uint64_t *)&core.reg.eax,
      (uint64_t *)&core.reg.ax,   (uint64_t *)&core.reg.al,
      (uint64_t *)&core.reg.rbx,  (uint64_t *)&core.reg.ebx,
      (uint64_t *)&core.reg.bx,   (uint64_t *)&core.reg.bl,
      (uint64_t *)&core.reg.rcx,  (uint64_t *)&core.reg.ecx,
      (uint64_t *)&core.reg.cx,   (uint64_t *)&core.reg.cl,
      (uint64_t *)&core.reg.rdx,  (uint64_t *)&core.reg.edx,
      (uint64_t *)&core.reg.dx,   (uint64_t *)&core.reg.dl,
      (uint64_t *)&core.reg.rsi,  (uint64_t *)&core.reg.esi,
      (uint64_t *)&core.reg.si,   (uint64_t *)&core.reg.sil,
      (uint64_t *)&core.reg.rdi,  (uint64_t *)&core.reg.edi,
      (uint64_t *)&core.reg.di,   (uint64_t *)&core.reg.dil,
      (uint64_t *)&core.reg.rbp,  (uint64_t *)&core.reg.ebp,
      (uint64_t *)&core.reg.bp,   (uint64_t *)&core.reg.bpl,
      (uint64_t *)&core.reg.rsp,  (uint64_t *)&core.reg.esp,
      (uint64_t *)&core.reg.sp,   (uint64_t *)&core.reg.spl,
      (uint64_t *)&core.reg.r8,   (uint64_t *)&core.reg.r8d,
      (uint64_t *)&core.reg.r8w,  (uint64_t *)&core.reg.r8b,
      (uint64_t *)&core.reg.r9,   (uint64_t *)&core.reg.r9d,
      (uint64_t *)&core.reg.r9w,  (uint64_t *)&core.reg.r9b,
      (uint64_t *)&core.reg.r10,  (uint64_t *)&core.reg.r10d,
      (uint64_t *)&core.reg.r10w, (uint64_t *)&core.reg.r10b,
      (uint64_t *)&core.reg.r11,  (uint64_t *)&core.reg.r11d,
      (uint64_t *)&core.reg.r11w, (uint64_t *)&core.reg.r11b,
      (uint64_t *)&core.reg.r12,  (uint64_t *)&core.reg.r12d,
      (uint64_t *)&core.reg.r12w, (uint64_t *)&core.reg.r12b,
      (uint64_t *)&core.reg.r13,  (uint64_t *)&core.reg.r13d,
      (uint64_t *)&core.reg.r13w, (uint64_t *)&core.reg.r13b,
      (uint64_t *)&core.reg.r14,  (uint64_t *)&core.reg.r14d,
      (uint64_t *)&core.reg.r14w, (uint64_t *)&core.reg.r14b,
      (uint64_t *)&core.reg.r15,  (uint64_t *)&core.reg.r15d,
      (uint64_t *)&core.reg.r15w, (uint64_t *)&core.reg.r15b
  };

  for (size_t i = 0; i < reg_size; ++i) {
    operand_t op;
    parse_operand(reg_operand_list[i], &core, &op);
    EXPECT_U32_EQ(op.type, REG);
    EXPECT_PTR_EQ(op.reg_b, cmp_reg_addr_list[i]);
    EXPECT_PTR_EQ(op.reg_i, NULL);
  }
}
TEST_END

TEST_BEGIN(test_parse_memory_operand_type) {
  size_t mem_size = 43;

  char *mem_operand_list[] = {
      "0x1234",
      "0xabcd",
      "1234",
      "(%rax)",
      "(%rbx)",
      "(%eax)",
      "(%rdi)",
      "(%esi)",
      "0xabcd(%rax)",
      "0xabcd(%rbx)",
      "0xabcd(%eax)",
      "1234(%rdi)",
      "1234(%esi)",
      "(%rax,%rbx)",
      "(%rbx,%esp)",
      "(%eax,%rcx)",
      "(%rdi,%rdx)",
      "(%esi,%rsi)",
      "0xabcd(%rax,%rbx)",
      "0xabcd(%rbx,%esp)",
      "0xabcd(%eax,%rcx)",
      "1234(%rdi,%rdx)",
      "1234(%esi,%rsi)",
      "(,%rbx,1)",
      "(,%esp,2)",
      "(,%rcx,4)",
      "(,%rdx,8)",
      "(,%rsi,1)",
      "0xabcd(,%rbx,1)",
      "0xabcd(,%esp,2)",
      "0xabcd(,%rcx,4)",
      "1234(,%rdx,8)",
      "1234(,%rsi,1)",
      "(%rax,%rbx,1)",
      "(%rbx,%esp,2)",
      "(%eax,%rcx,4)",
      "(%rdi,%rdx,8)",
      "(%esi,%rsi,1)",
      "0xabcd(%rax,%rbx,1)",
      "0xabcd(%rbx,%esp,2)",
      "0xabcd(%eax,%rcx,4)",
      "1234(%rdi,%rdx,8)",
      "1234(%esi,%rsi,1)"
  };

  operand_t cmp_op_list[] = {
      {MEM, 0x1234, 1, NULL, NULL},
      {MEM, 0xabcd, 1, NULL, NULL},
      {MEM, 1234, 1, NULL, NULL},
      {MEM, 0, 1, (uint64_t *)&core.reg.rax, NULL},
      {MEM, 0, 1, (uint64_t *)&core.reg.rbx, NULL},
      {MEM, 0, 1, (uint64_t *)&core.reg.eax, NULL},
      {MEM, 0, 1, (uint64_t *)&core.reg.rdi, NULL},
      {MEM, 0, 1, (uint64_t *)&core.reg.esi, NULL},
      {MEM, 0xabcd, 1, (uint64_t *)&core.reg.rax, NULL},
      {MEM, 0xabcd, 1, (uint64_t *)&core.reg.rbx, NULL},
      {MEM, 0xabcd, 1, (uint64_t *)&core.reg.eax, NULL},
      {MEM, 1234, 1, (uint64_t *)&core.reg.rdi, NULL},
      {MEM, 1234, 1, (uint64_t *)&core.reg.esi, NULL},
      {MEM, 0, 1, (uint64_t *)&core.reg.rax, (uint64_t *)&core.reg.rbx},
      {MEM, 0, 1, (uint64_t *)&core.reg.rbx, (uint64_t *)&core.reg.esp},
      {MEM, 0, 1, (uint64_t *)&core.reg.eax, (uint64_t *)&core.reg.rcx},
      {MEM, 0, 1, (uint64_t *)&core.reg.rdi, (uint64_t *)&core.reg.rdx},
      {MEM, 0, 1, (uint64_t *)&core.reg.esi, (uint64_t *)&core.reg.rsi},
      {MEM, 0xabcd, 1, (uint64_t *)&core.reg.rax, (uint64_t *)&core.reg.rbx},
      {MEM, 0xabcd, 1, (uint64_t *)&core.reg.rbx, (uint64_t *)&core.reg.esp},
      {MEM, 0xabcd, 1, (uint64_t *)&core.reg.eax, (uint64_t *)&core.reg.rcx},
      {MEM, 1234, 1, (uint64_t *)&core.reg.rdi, (uint64_t *)&core.reg.rdx},
      {MEM, 1234, 1, (uint64_t *)&core.reg.esi, (uint64_t *)&core.reg.rsi},
      {MEM, 0, 1, NULL, (uint64_t *)&core.reg.rbx},
      {MEM, 0, 2, NULL, (uint64_t *)&core.reg.esp},
      {MEM, 0, 4, NULL, (uint64_t *)&core.reg.rcx},
      {MEM, 0, 8, NULL, (uint64_t *)&core.reg.rdx},
      {MEM, 0, 1, NULL, (uint64_t *)&core.reg.rsi},
      {MEM, 0xabcd, 1, NULL, (uint64_t *)&core.reg.rbx},
      {MEM, 0xabcd, 2, NULL, (uint64_t *)&core.reg.esp},
      {MEM, 0xabcd, 4, NULL, (uint64_t *)&core.reg.rcx},
      {MEM, 1234, 8, NULL, (uint64_t *)&core.reg.rdx},
      {MEM, 1234, 1, NULL, (uint64_t *)&core.reg.rsi},
      {MEM, 0, 1, (uint64_t *)&core.reg.rax, (uint64_t *)&core.reg.rbx},
      {MEM, 0, 2, (uint64_t *)&core.reg.rbx, (uint64_t *)&core.reg.esp},
      {MEM, 0, 4, (uint64_t *)&core.reg.eax, (uint64_t *)&core.reg.rcx},
      {MEM, 0, 8, (uint64_t *)&core.reg.rdi, (uint64_t *)&core.reg.rdx},
      {MEM, 0, 1, (uint64_t *)&core.reg.esi, (uint64_t *)&core.reg.rsi},
      {MEM, 0xabcd, 1, (uint64_t *)&core.reg.rax, (uint64_t *)&core.reg.rbx},
      {MEM, 0xabcd, 2, (uint64_t *)&core.reg.rbx, (uint64_t *)&core.reg.esp},
      {MEM, 0xabcd, 4, (uint64_t *)&core.reg.eax, (uint64_t *)&core.reg.rcx},
      {MEM, 1234, 8, (uint64_t *)&core.reg.rdi, (uint64_t *)&core.reg.rdx},
      {MEM, 1234, 1, (uint64_t *)&core.reg.esi, (uint64_t *)&core.reg.rsi}
  };

  for (size_t i = 0; i < mem_size; ++i) {
    operand_t op;
    parse_operand(mem_operand_list[i], &core, &op);
    EXPECT_U32_EQ(op.type, MEM);
    EXPECT_PTR_EQ(op.reg_b, cmp_op_list[i].reg_b);
    EXPECT_PTR_EQ(op.reg_i, cmp_op_list[i].reg_i);
    EXPECT_D64_EQ(op.imm, cmp_op_list[i].imm);
    EXPECT_D64_EQ(op.scal, cmp_op_list[i].scal);
  }
}
TEST_END

int main(void) {
  return RUN_TESTS(test_parse_operand_immediate_number,
                   test_parse_operand_register, 
                   test_parse_memory_operand_type);
}