#include <stdio.h>

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
  "push   %rbp",
  "mov    %rsp,%rbp",
  "sub    $0x10,%rsp", rsp - 10
  "mov    %rdi,-0x8(%rbp)", rsp - 8
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
  "push   %rbp",
  "mov    %rsp,%rbp",
  "sub    $0x10,%rsp", rsp - 10
  "mov    $0x3,%rdi",
  "callq  $0x00400000",
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
  dram_write(va2pa(core.reg.rsp - 0x28), 0x0);             // n = 3 saved here
  dram_write(va2pa(core.reg.rsp - 0x8), 0x7fffff7912e8);
  dram_write(va2pa(core.reg.rsp), 0x7fffff5c40b3);
}

// register compare list
uint64_t rbp_cmp_list[CODE_SIZE] = {
  0x0,
};

uint64_t rax_cmp_list[CODE_SIZE] = {
  0x8001160,
};

uint64_t rdx_cmp_list[CODE_SIZE] = {
  0x7ffffffedf78,
};

uint64_t rdi_cmp_list[CODE_SIZE] = {
  0x1,
};

uint64_t rsi_cmp_list[CODE_SIZE] = {
  0x7ffffffedf68,
};

uint64_t rsp_cmp_list[CODE_SIZE] = {
  0x7ffffffede70,
};

uint64_t rip_cmp_list[CODE_SIZE] = {
  
};

// memory verify
uint64_t mem_cmp_list[6][CODE_SIZE] = {
  // 0x7ffffffede70
  {
    0x0, 
  },
  // 0x7ffffffede50
  {
    0x0, 
  },
  // 0x7ffffffede50
  {
    0x0, 
  },
};