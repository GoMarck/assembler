#include <stdio.h>

#include "assembler/mmu.h"
#include "assembler/cpu.h"
#include "assembler/dram.h"
#include "assembler/inst.h"

extern Instruction program[20];

#define MAX_INSTRUCTION_SIZE 128

char assembly[18][MAX_INSTRUCTION_SIZE] = {
  "push   %rbp",
  "mov    %rsp,%rbp",
  "mov    %rdi,-0x18(%rbp)",
  "mov    %rsi,-0x20(%rbp)",
  "mov    -0x18(%rbp),%rdx",
  "mov    -0x20(%rbp),%rax",
  "add    %rdx,%rax",
  "mov    %rax,-0x8(%rbp)",
  "mov    -0x8(%rbp),%rax",
  "movq   $0x12340000,-0x18(%rbp)",
  "movq   $0xabcd,-0x10(%rbp)",
  "mov    -0x10(%rbp),%rdx",
  "mov    -0x18(%rbp),%rax",
  "mov    %rdx,%rsi",
  "mov    %rax,%rdi",
  "callq  0",
  "mov    %rax,-0x8(%rbp)",
  "mov    $0x0,%eax"
};

// init register state
void init_reg();

// init memory state
void init_mem();

// print register state
void print_reg();

// print memory state around rsp
void print_mem();

int main() {
  init_handler_table();
  init_reg();
  init_mem();
  for (int i = 0; i < 20; ++i) {
    parse_inst();
    print_reg();
    print_mem();
  }
  return 0;
}

void init_reg() {
  core.reg.rax = 0x800114e;
  core.reg.rbx = 0x8001190;
  core.reg.rcx = 0x8001190;
  core.reg.rdx = 0x7ffffffee388;
  core.reg.rsi = 0x7ffffffee378;
  core.reg.rdi = 0x1;
  core.reg.rbp = 0x7ffffffee280;
  core.reg.rsp = 0x7ffffffee260;
  core.rip = (uint64_t)&program[11];
}

void init_mem() {
  dram_write(va2pa(0x7ffffffee260), 0x0);  // rsp
  dram_write(va2pa(0x7ffffffee268), 0x08001040);
  dram_write(va2pa(0x7ffffffee270), 0x00007ffffffee370);
  dram_write(va2pa(0x7ffffffee278), 0x0);
  dram_write(va2pa(0x7ffffffee280), 0x0);  // rbp
}

void print_reg() {
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

void print_mem() {
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