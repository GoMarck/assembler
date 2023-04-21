#include <stdio.h>

#include "assembler/log.h"
#include "assembler/cpu.h"
#include "assembler/dram.h"
#include "assembler/inst.h"
#include "assembler/mmu.h"
#include "test/test.h"

#define CODE_SIZE 25

// This is the assembly code translated from a simple C program:
//
// #include <stdint.h>

// uint64_t sum(uint64_t n)
// {
//     if (n == 0) {
//         return 0;
//     } else {
//         return n + sum(n - 1);
//     }
// }

// int main()
// {
//     uint64_t a = sum(3);
//     return 0;
// }
char assembly_code[CODE_SIZE][INSTRUCTION_SIZE] = {
  // <sum>
  "push   %rbp",  // rsp - 8
  "mov    %rsp,%rbp",
  "sub    $0x10,%rsp", // rsp - 10
  "mov    %rdi,-0x8(%rbp)", // rsp - 8
  "cmpq   $0x0,-0x8(%rbp)",
  "jne    $0x00400200",
  "mov    $0x0,%rax",
  "jmp    $0x00400380",
  "mov    -0x8(%rbp),%rax",
  "sub    $0x1,%rax",
  "mov    %rax,%rdi",
  "callq  $0x00400000",
  "mov    -0x8(%rbp),%rdx",
  "add    %rdx,%rax",
  "leaveq",
  "retq",
  // <main>
  "push   %rbp", // rsp - 8
  "mov    %rsp,%rbp",
  "sub    $0x10,%rsp", // rsp - 10
  "mov    $0x3,%rdi",
  "callq  $0x00400000",  // rsp - 8
  "mov    %rax,-0x8(%rbp)",
  "mov    $0x0,%rax",
  "leaveq",
  "retq"
};

void init_assembly_code() {
  uint64_t code_mem_addr = 0x00400000;
  for (size_t i = 0; i < CODE_SIZE; ++i) {
    dram_write_instruction(va2pa(code_mem_addr), assembly_code[i], INSTRUCTION_SIZE);
    code_mem_addr += INSTRUCTION_SIZE;
  }
}

void init_register() {
  core.reg.rax = 0x8001160;
  core.reg.rbx = 0x8001190;
  core.reg.rcx = 0x8001190;
  core.reg.rdx = 0x7ffffffedf78;
  core.reg.rsi = 0x7ffffffedf68;
  core.reg.rdi = 0x1;
  core.reg.rbp = 0x0;
  core.reg.rsp = 0x7ffffffede78;
  core.rip = 0x00400400;
}

void init_memory() {
  init_assembly_code();
  dram_write(va2pa(core.reg.rsp - 0x58), 0xc2);            // n = 0 saved here
  dram_write(va2pa(core.reg.rsp - 0x48), 0x7ffffffede56);  // n = 1 saved here
  dram_write(va2pa(core.reg.rsp - 0x38), 0x7fffff7912e8);  // n = 2 saved here
  dram_write(va2pa(core.reg.rsp - 0x30), 0x80011dd);       // n = 3 saved here
  dram_write(va2pa(core.reg.rsp - 0x20), 0x8001040);
  dram_write(va2pa(core.reg.rsp - 0x8), 0x8001190);
  dram_write(va2pa(core.reg.rsp), 0x7fffff5c40b3);
}

// register compare list
uint64_t rbp_cmp_list[CODE_SIZE] = {
  0x0, 0x7ffffffede70, 0x7ffffffede70, 0x7ffffffede70, 0x7ffffffede70,
  0x7ffffffede70, 0x7ffffffede50, 0x7ffffffede50, 0x7ffffffede50
};

uint64_t rax_cmp_list[CODE_SIZE] = {
  0x8001160, 0x8001160, 0x8001160, 0x8001160, 0x8001160, 0x8001160,
  0x8001160, 0x8001160, 0x8001160
};

uint64_t rdx_cmp_list[CODE_SIZE] = {
  0x7ffffffedf78, 0x7ffffffedf78, 0x7ffffffedf78, 0x7ffffffedf78,
  0x7ffffffedf78, 0x7ffffffedf78, 0x7ffffffedf78, 0x7ffffffedf78,
  0x7ffffffedf78
};

uint64_t rdi_cmp_list[CODE_SIZE] = {
  0x1, 0x1, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3
};

uint64_t rsi_cmp_list[CODE_SIZE] = {
  0x7ffffffedf68, 0x7ffffffedf68, 0x7ffffffedf68, 0x7ffffffedf68, 
  0x7ffffffedf68, 0x7ffffffedf68, 0x7ffffffedf68, 0x7ffffffedf68,
  0x7ffffffedf68
};

uint64_t rsp_cmp_list[CODE_SIZE] = {
  0x7ffffffede70, 0x7ffffffede70, 0x7ffffffede60, 0x7ffffffede60, 
  0x7ffffffede58, 0x7ffffffede50, 0x7ffffffede50, 0x7ffffffede40,
  0x7ffffffede40
};

uint64_t rip_cmp_list[CODE_SIZE] = {
  0x00400440, 0x00400480, 0x004004c0, 0x00400500, 0x00400000,
  0x00400040, 0x00400080, 0x004000c0, 0x00400100
};

void register_verify(int i) {
  EXPECT_U64_EQ(core.reg.rbp, rbp_cmp_list[i]);
  EXPECT_U64_EQ(core.reg.rax, rax_cmp_list[i]);
  EXPECT_U64_EQ(core.reg.rdx, rdx_cmp_list[i]);
  EXPECT_U64_EQ(core.reg.rdi, rdi_cmp_list[i]);
  EXPECT_U64_EQ(core.reg.rsi, rsi_cmp_list[i]);
  EXPECT_U64_EQ(core.reg.rsp, rsp_cmp_list[i]);
  EXPECT_U64_EQ(core.rip, rip_cmp_list[i]);
}

// memory verify
uint64_t mem_cmp_list[6][CODE_SIZE] = {
  // 0x7ffffffede70
  {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,0x0
  },
  // 0x7ffffffede58
  {
    0x8001040, 0x8001040, 0x8001040, 0x8001040, 0x00400540,
    0x00400540, 0x00400540, 0x00400540,0x00400540
  },
  // 0x7ffffffede50
  {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x7ffffffede70, 0x7ffffffede70,
    0x7ffffffede70, 0x7ffffffede70
  },
  // 0x7ffffffede48
  {
    0x80011dd, 0x80011dd, 0x80011dd, 0x80011dd, 0x80011dd, 
    0x80011dd, 0x80011dd, 0x80011dd, 0x3
  }
};

void memory_verify(int i) {
  EXPECT_U64_EQ(dram_read(va2pa(0x7ffffffede70)), mem_cmp_list[0][i]);
  EXPECT_U64_EQ(dram_read(va2pa(0x7ffffffede58)), mem_cmp_list[1][i]);
  EXPECT_U64_EQ(dram_read(va2pa(0x7ffffffede50)), mem_cmp_list[2][i]);
  EXPECT_U64_EQ(dram_read(va2pa(0x7ffffffede48)), mem_cmp_list[3][i]);
}

void instruction_cycle() {
  for (int i = 0; i < 9; ++i) {
    parse_instruction();
    register_verify(i);
    memory_verify(i);
  }
}

TEST_BEGIN(recursive_assembler_code_st) {
  init_handler_table();
  init_register();
  init_memory();
  instruction_cycle();
}
TEST_END

int main(void) {
  return RUN_TESTS(recursive_assembler_code_st); 
}