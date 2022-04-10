#include <stdio.h>

#include "cpu/mmu.h"
#include "cpu/reg.h"
#include "memory/dram.h"
#include "memory/inst.h"

extern Instruction program[20];

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
  reg.rax = 0x800114e;
  reg.rbx = 0x8001190;
  reg.rcx = 0x8001190;
  reg.rdx = 0x7ffffffee388;
  reg.rsi = 0x7ffffffee378;
  reg.rdi = 0x1;
  reg.rbp = 0x7ffffffee280;
  reg.rsp = 0x7ffffffee260;
  reg.rip = (uint64_t)&program[11];
}

void init_mem() {
  dram_write(va2pa(0x7ffffffee260), 0x0);  // rsp
  dram_write(va2pa(0x7ffffffee268), 0x08001040);
  dram_write(va2pa(0x7ffffffee270), 0x00007ffffffee370);
  dram_write(va2pa(0x7ffffffee278), 0x0);
  dram_write(va2pa(0x7ffffffee280), 0x0);  // rbp
}

void print_reg() {
  printf("rax          0x%lx\n", reg.rax);
  printf("rbx          0x%lx\n", reg.rbx);
  printf("rcx          0x%lx\n", reg.rcx);
  printf("rdx          0x%lx\n", reg.rdx);
  printf("rsi          0x%lx\n", reg.rsi);
  printf("rdi          0x%lx\n", reg.rdi);
  printf("rbp          0x%lx\n", reg.rbp);
  printf("rsp          0x%lx\n", reg.rsp);
  printf("rip          0x%lx\n", reg.rip);
}

void print_mem() {
  int n = 5;
  uint64_t start = reg.rsp + n * 8;
  for (int i = 0; i < 2 * n; ++i) {
    uint64_t addr = start - i * 8;
    printf("0x%016lx : %16lx", addr, dram_read(va2pa(addr)));
    if (addr == reg.rsp) {
      printf("  <== rsp");
    }
    printf("\n");
  }
}