#include "memory/inst.h"

#include <string.h>

#include "cpu/reg.h"

#define INST_LEN 20

inst_t program[INST_LEN] = {
    // add
    { { REG, 0, 0, &reg.rbp, NULL }, { NUL, 0, 0, NULL, NULL }, PUSH, "push   \%rbp" },
    { { REG, 0, 0, &reg.rsp, NULL }, { REG, 0, 0, &reg.rbp, NULL }, MOV_REG_REG, "mov    \%rsp,\%rbp" },
    { { REG, 0, 0, &reg.rdi, NULL }, { MEM, -0x18, 0, &reg.rbp, NULL }, MOV_REG_MEM, "mov    \%rdi,-0x18(\%rbp)" },
    { { REG, 0, 0, &reg.rsi, NULL }, { MEM, -0x20, 0, &reg.rbp, NULL }, MOV_REG_MEM, "mov    \%rsi,-0x20(\%rbp)" },
    { { MEM, -0x18, 0, &reg.rbp, NULL }, { REG, 0, 0, &reg.rdx, NULL }, MOV_MEM_REG, "mov    -0x18(\%rbp),\%rdx" },
    { { MEM, -0x20, 0, &reg.rbp, NULL }, { REG, 0, 0, &reg.rax, NULL }, MOV_MEM_REG, "mov    -0x20(\%rbp),\%rax" },
    { { REG, 0, 0, &reg.rdx, NULL }, { REG, 0, 0, &reg.rax, NULL }, ADD_REG_REG, "add    \%rdx,\%rax" },
    { { REG, 0, 0, &reg.rax, NULL }, { MEM, -0x8, 0, &reg.rbp, NULL }, MOV_REG_MEM, "mov    \%rax,-0x8(\%rbp)" },
    { { MEM, -0x8, 0, &reg.rbp, NULL }, { REG, 0, 0, &reg.rax, NULL }, MOV_MEM_REG, "mov    -0x8(\%rbp),\%rax" },
    { { REG, 0, 0, &reg.rbp, NULL }, { NUL, 0, 0, NULL, NULL }, POP, "pop    \%rbp" },
    { { NUL, 0, 0, NULL, NULL }, { NUL, 0, 0, NULL, NULL }, RET, "retq" },
    // main
    { { IMM, 0x12340000, 0, NULL, NULL }, { MEM, -0x18, 0, &reg.rbp, NULL }, MOV_IMM_MEM, "movq   $0x12340000,-0x18(\%rbp)" },
    { { IMM, 0xabcd, 0, NULL, NULL }, { MEM, -0x10, 0, &reg.rbp, NULL }, MOV_IMM_MEM, "movq   $0xabcd,-0x10(\%rbp)" },
    { { MEM, -0x10, 0, &reg.rbp, NULL }, { REG, 0, 0, &reg.rdx, NULL }, MOV_MEM_REG, "mov    -0x10(\%rbp),\%rdx" },
    { { MEM, -0x18, 0, &reg.rbp, NULL }, { REG, 0, 0, &reg.rax, NULL }, MOV_MEM_REG, "mov    -0x18(\%rbp),\%rax" },
    { { REG, 0, 0, &reg.rdx, NULL }, { REG, 0, 0, &reg.rsi, NULL }, MOV_REG_REG, "mov    \%rdx,\%rsi" },
    { { REG, 0, 0, &reg.rax, NULL }, { REG, 0, 0, &reg.rdi, NULL }, MOV_REG_REG, "mov    \%rax,\%rdi" },
    { { IMM, (int64_t)&program, 0, NULL, NULL }, { NUL, 0, 0, NULL, NULL }, CALL, "callq  1129 <add>" },
    { { REG, 0, 0, &reg.rax, NULL }, { MEM, -0x8, 0, &reg.rbp, NULL }, MOV_REG_MEM, "mov    \%rax,-0x8(\%rbp)" },
    { { IMM, 0, 0, NULL, NULL }, { REG, 0, 0, &reg.rax, NULL }, MOV_IMM_REG, "mov    $0x0,\%eax" }
};