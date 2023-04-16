#include <bits/stdint-uintn.h>
#include <stdio.h>

#include "assembler/cpu.h"
#include "assembler/dram.h"
#include "assembler/inst.h"
#include "assembler/mmu.h"
#include "test/test.h"

#define CODE_SIZE 20

// This is the assembly code translated from a simple C program:
//
// int64_t add(int64_t a, int64_t b)
// {
//     int64_t c = a + b;
//     return c;
// }

// int64_t main()
// {
//     int64_t a = 0x12340000;
//     int64_t b = 0xabcd;
//     int64_t c = add(a, b);
//     return 0;
// }
char assembly_code[CODE_SIZE][INSTRUCTION_SIZE] = {
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
  "callq  $0x00400000",
  "mov    %rax,-0x8(%rbp)",
  "mov    $0x0,%eax"
};

void init_assembly_code() {
  uint64_t code_mem_addr = 0x00400000;
  for (size_t i = 0; i < CODE_SIZE; ++i) {
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
  core.rip = 0x004002c0;
}

void init_memory() {
  init_assembly_code();
  dram_write(va2pa(core.reg.rsp), 0x0);
  dram_write(va2pa(0x7ffffffee268), 0x08001040);
  dram_write(va2pa(0x7ffffffee270), 0x00007ffffffee370);
  dram_write(va2pa(0x7ffffffee278), 0x0);
  dram_write(va2pa(core.reg.rbp), 0x0);
  dram_write(va2pa(0x7ffffffee248), 0x0);
  dram_write(va2pa(0x7ffffffee238), 0x0);
  dram_write(va2pa(0x7ffffffee230), 0x0);
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

// register compare list
uint64_t rbp_cmp_list[CODE_SIZE] = {
  0x7ffffffee280, 0x7ffffffee280, 0x7ffffffee280,
  0x7ffffffee280, 0x7ffffffee280, 0x7ffffffee280,
  0x7ffffffee280, 0x7ffffffee280, 0x7ffffffee250,
  0x7ffffffee250, 0x7ffffffee250, 0x7ffffffee250,
  0x7ffffffee250, 0x7ffffffee250, 0x7ffffffee250,
  0x7ffffffee250, 0x7ffffffee280, 0x7ffffffee280,
  0x7ffffffee280, 0x7ffffffee280
};

uint64_t rax_cmp_list[CODE_SIZE] = {
  0x800114e,  0x800114e,  0x800114e,  0x12340000,
  0x12340000, 0x12340000, 0x12340000, 0x12340000,
  0x12340000, 0x12340000, 0x12340000, 0x12340000,
  0xabcd,     0x1234abcd, 0x1234abcd, 0x1234abcd,
  0x1234abcd, 0x1234abcd, 0x1234abcd, 0x0
};

uint64_t rdx_cmp_list[CODE_SIZE] = {
  0x7ffffffee388, 0x7ffffffee388, 0xabcd,
  0xabcd, 0xabcd, 0xabcd, 0xabcd, 0xabcd,
  0xabcd, 0xabcd, 0xabcd, 0x12340000, 0x12340000,
  0x12340000, 0x12340000, 0x12340000, 0x12340000,
  0x12340000, 0x12340000, 0x12340000
};

uint64_t rdi_cmp_list[CODE_SIZE] = {
  0x1, 0x1, 0x1, 0x1, 0x1, 0x12340000, 0x12340000, 0x12340000,
  0x12340000, 0x12340000, 0x12340000, 0x12340000, 0x12340000,
  0x12340000, 0x12340000, 0x12340000, 0x12340000, 0x12340000,
  0x12340000, 0x12340000
};

uint64_t rsi_cmp_list[CODE_SIZE] = {
  0x7ffffffee378, 0x7ffffffee378, 0x7ffffffee378, 
  0x7ffffffee378, 0xabcd, 0xabcd, 0xabcd, 0xabcd,
  0xabcd, 0xabcd, 0xabcd, 0xabcd, 0xabcd, 0xabcd,
  0xabcd, 0xabcd, 0xabcd, 0xabcd, 0xabcd, 0xabcd
};

uint64_t rsp_cmp_list[CODE_SIZE] = {
  0x7ffffffee260, 0x7ffffffee260, 0x7ffffffee260,
  0x7ffffffee260, 0x7ffffffee260, 0x7ffffffee260,
  0x7ffffffee258, 0x7ffffffee250, 0x7ffffffee250, 
  0x7ffffffee250, 0x7ffffffee250, 0x7ffffffee250, 
  0x7ffffffee250, 0x7ffffffee250, 0x7ffffffee250, 
  0x7ffffffee250, 0x7ffffffee250, 0x7ffffffee258,
  0x7ffffffee260, 0x7ffffffee260
};

void register_verify(int i) {
  EXPECT_U64_EQ(core.reg.rbp, rbp_cmp_list[i]);
  EXPECT_U64_EQ(core.reg.rax, rax_cmp_list[i]);
  EXPECT_U64_EQ(core.reg.rdx, rdx_cmp_list[i]);
  EXPECT_U64_EQ(core.reg.rdi, rdi_cmp_list[i]);
  EXPECT_U64_EQ(core.reg.rsi, rsi_cmp_list[i]);
}

// memory verify
uint64_t mem_cmp_list[6][CODE_SIZE] = {
  {
    0x12340000, 0x12340000, 0x12340000, 0x12340000,
    0x12340000, 0x12340000, 0x12340000, 0x12340000,
    0x12340000, 0x12340000, 0x12340000, 0x12340000,
    0x12340000, 0x12340000, 0x12340000, 0x12340000,
    0x12340000, 0x12340000, 0x12340000, 0x12340000
  },
  {
    0x00007ffffffee370, 0xabcd, 0xabcd, 0xabcd, 0xabcd,
    0xabcd, 0xabcd, 0xabcd, 0xabcd, 0xabcd, 0xabcd, 
    0xabcd, 0xabcd, 0xabcd, 0xabcd, 0xabcd, 0xabcd, 
    0xabcd, 0xabcd, 0xabcd
  },
  {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x1234abcd, 0x1234abcd
  },
  {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x1234abcd, 0x1234abcd,
    0x1234abcd, 0x1234abcd, 0x1234abcd, 0x1234abcd
  },
  {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x12340000, 0x12340000, 0x12340000, 0x12340000,
    0x12340000, 0x12340000, 0x12340000, 0x12340000,
    0x12340000, 0x12340000, 0x12340000
  },
  {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xabcd, 0xabcd, 0xabcd, 0xabcd, 0xabcd,
    0xabcd, 0xabcd, 0xabcd, 0xabcd, 0xabcd
  }
};

void memory_verify(int i) {
  EXPECT_U64_EQ(dram_read(va2pa(0x7ffffffee268)), mem_cmp_list[0][i]);
  EXPECT_U64_EQ(dram_read(va2pa(0x7ffffffee270)), mem_cmp_list[1][i]);
  EXPECT_U64_EQ(dram_read(va2pa(0x7ffffffee278)), mem_cmp_list[2][i]);
  EXPECT_U64_EQ(dram_read(va2pa(0x7ffffffee248)), mem_cmp_list[3][i]);
  EXPECT_U64_EQ(dram_read(va2pa(0x7ffffffee238)), mem_cmp_list[4][i]);
  EXPECT_U64_EQ(dram_read(va2pa(0x7ffffffee230)), mem_cmp_list[5][i]);
}

void instruction_cycle() {
  for (int i = 0; i < CODE_SIZE; ++i) {
    parse_instruction();
    register_verify(i);
    memory_verify(i);
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