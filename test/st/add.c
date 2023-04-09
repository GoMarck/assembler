#include <stdio.h>

#include "assembler/cpu.h"
#include "assembler/dram.h"
#include "assembler/inst.h"
#include "assembler/mmu.h"
#include "test/test.h"

char assembly_code[20][INSTRUCTION_SIZE] = {
  // add
  "push   %rbp",
  "mov    %rsp,%rbp",
  "mov    %rdi,-0x18(%rbp)",
  "mov    %rsi,-0x20(%rbp)",
  "mov    -0x18(%rbp),%rdx",
  "mov    -0x20(%rbp),%rax",
  "add    %rdx,%rax",
  "mov    %rax,-0x8(%rbp)",
  "mov    -0x8(%rbp),%rax",
  "pop    %rbp",
  "retq",
  // main
  "movq   $0x12340000,-0x18(%rbp)",
  "movq   $0xabcd,-0x10(%rbp)",
  "mov    -0x10(%rbp),%rdx",
  "mov    -0x18(%rbp),%rax",
  "mov    %rdx,%rsi",
  "mov    %rax,%rdi",
  "callq  0x00400000",
  "mov    %rax,-0x8(%rbp)",
  "mov    $0x0,%eax"
};

void init_assembly_code() {
  uint64_t code_mem_addr = 0x004002c0;
  for (size_t i = 0; i < 18; ++i) {
    dram_write_instruction(va2pa(code_mem_addr), assembly_code[i], INSTRUCTION_SIZE);
    code_mem_addr += INSTRUCTION_SIZE;
  }
}

void init_register() {
  core.reg.rax = 0x800114e;
  core.reg.rbx = 0x8001190;
  core.reg.rcx = 0x8001190;
  core.reg.rdx = 0x7ffffffee388;
  core.reg.rsi = 0x7ffffffee378;
  core.reg.rdi = 0x1;
  core.reg.rbp = 0x7ffffffee280;
  core.reg.rsp = 0x7ffffffee260;
  core.rip = 0x00400000;
}

void init_memory() {
  init_assembly_code();
  dram_write(va2pa(core.reg.rsp), 0x0);
  dram_write(va2pa(0x7ffffffee268), 0x08001040);
  dram_write(va2pa(0x7ffffffee270), 0x00007ffffffee370);
  dram_write(va2pa(0x7ffffffee278), 0x0);
  dram_write(va2pa(core.reg.rbp), 0x0);
}

void print_register() {
  printf("rax          0x%lx\n", core.reg.rax);
  printf("rbx          0x%lx\n", core.reg.rbx);
  printf("rcx          0x%lx\n", core.reg.rcx);
  printf("rdx          0x%lx\n", core.reg.rdx);
  printf("rsi          0x%lx\n", core.reg.rsi);
  printf("rdi          0x%lx\n", core.reg.rdi);
  printf("rbp          0x%lx\n", core.reg.rbp);
  printf("rsp          0x%lx\n", core.reg.rsp);
  printf("rip          0x%lx\n", core.rip);
}

void print_memory() {
  int n = 5;
  uint64_t start = core.reg.rsp + n * 8;
  for (int i = 0; i < 2 * n; ++i) {
    uint64_t addr = start - i * 8;
    printf("0x%016lx : %16lx", addr, dram_read(va2pa(addr)));
    if (addr == core.reg.rsp) {
      printf("  <== rsp");
    }
    printf("\n");
  }
}

void instruction_cycle() {
  for (int i = 0; i < 20; ++i) {
    printf("===========================%d==================================\n", i);
    parse_instruction();
    print_register();
    print_memory();
  }
}

TEST_BEGIN(add_assembler_code_st) {
  init_handler_table();
  init_register();
  init_memory();
  instruction_cycle();
}
TEST_END

int main(void) { 
  return RUN_TESTS(add_assembler_code_st); 
}