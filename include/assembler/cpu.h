#pragma once

#ifndef ASSEMBLER_CPU_H
#define ASSEMBLER_CPU_H

#include <stdbool.h>
#include <stdint.h>

#define REG_DEFINE(r1, r2, r3, r4) \
  union {                          \
    uint64_t (r1);                 \
    uint32_t (r2);                 \
    uint16_t (r3);                 \
    uint8_t  (r4);                 \
  }

// X86-64 CPU register struct. 
//
// It contains a set of 16 general-purpose registers storing 64-bit values. 
// These registers are used to store integer data as well as pointers. Their
// name and function are shown as follow:
// 
// 63                            31               15       7        0
// |%rax                         |%eax            |%ax     |%al     | --> Return value
// |%rbx                         |%ebx            |%bx     |%bl     | --> Callee saved
// |%rcx                         |%ecx            |%cx     |%cl     | --> 4th argument
// |%rdx                         |%edx            |%dx     |%dl     | --> 3rd argument
// |%rsi                         |%esi            |%si     |%sil    | --> 2nd argument
// |%rdi                         |%edi            |%di     |%dil    | --> 1st argument
// |%rbp                         |%ebp            |%bp     |%bpl    | --> Callee saved
// |%rsp                         |%esp            |%sp     |%spl    | --> Stack pointer
// |%r8                          |%r8d            |%r8w    |%r8b    | --> 5th argument
// |%r9                          |%r9d            |%r9w    |%r9b    | --> 6th argument
// |%r10                         |%r10d           |%r10w   |%r10b   | --> Callee saved
// |%r11                         |%r11d           |%r11w   |%r11b   | --> Callee saved
// |%r12                         |%r12d           |%r12w   |%r12b   | --> Callee saved
// |%r13                         |%r13d           |%r13w   |%r13b   | --> Callee saved
// |%r14                         |%r14d           |%r14w   |%r14b   | --> Callee saved
// |%r15                         |%r15d           |%r15w   |%r15b   | --> Callee saved
typedef struct reg_t {
  REG_DEFINE(rax, eax,  ax,   al);
  REG_DEFINE(rbx, ebx,  bx,   bl);
  REG_DEFINE(rcx, ecx,  cx,   cl);
  REG_DEFINE(rdx, edx,  dx,   dl);
  REG_DEFINE(rsi, esi,  si,   sil);
  REG_DEFINE(rdi, edi,  di,   dil);
  REG_DEFINE(rbp, ebp,  bp,   bpl);
  REG_DEFINE(rsp, esp,  sp,   spl);
  REG_DEFINE(r8,  r8d,  r8w,  r8b);
  REG_DEFINE(r9,  r9d,  r9w,  r9b);
  REG_DEFINE(r10, r10d, r10w, r10b);
  REG_DEFINE(r11, r11d, r11w, r11b);
  REG_DEFINE(r12, r12d, r12w, r12b);
  REG_DEFINE(r13, r13d, r13w, r13b);
  REG_DEFINE(r14, r14d, r14w, r14b);
  REG_DEFINE(r15, r15d, r15w, r15b);
} reg_t;

#define BOOL(flag) (bool)(flag)
#define BYTE(flag) (uint8_t)(flag)

// Marcos for read or write condition codes
//
//    * CF_{RD|WR}: Read or write CF flag
//    * ZF_{RD|WR}: Read or write ZF flag
//    * SF_{RD|WR}: Read or write SF flag
//    * OF_{RD|WR}: Read or write OF flag
//
// Condition code registers describing attributes of the
// most recent arithmetic or logical operation:
//    * CF: Carry flag. The most recent operation generated a carry out of
//          the most significant bit. Used to detect overflow for unsigned
//          operations.
//    * ZF: Zero flag. The most recent operation yielded zero.
//    * SF: Sign flag. The most recent operation yielded a negative value.
//    * OF: Overflow flag. The most recent operation caused a two’s-complement
//          overflow—either negative or positive.
//
// Example:
//    suppose use ADD instructions to perform the t = a + b, then the condition
//    codes would be set according to the following C expressions:
//    * CF   (unsigned) t < (unsigned) a            Unsigned overflow
//    * ZF   (t == 0)                               Zero
//    * SF   (t < 0)                                Negative
//    * OF   (a < 0 == b < 0) && (t < 0 != a < 0)   Signed overflow
#define CF_RD(cp) BOOL((cp)->codes & 0x1)
#define CF_WR(cp, flag) ((cp)->codes | BYTE(BOOL(flag)))
#define ZF_RD(cp) BOOL((cp)->codes & 0x10)
#define ZF_WR(cp, flag) ((cp)->codes | BYTE(BOOL(flag)) << 0x1))
#define SF_RD(cp) BOOL((cp)->codes & 0x100)
#define SF_WR(cp, flag) ((cp)->codes | BYTE(BOOL(flag)) << 0x10))
#define OF_RD(cp) BOOL((cp)->codes & 0x1000)
#define OF_WR(cp, flag) ((cp)->codes | BYTE(BOOL(flag)) << 0x100))

typedef struct core_t {
  reg_t reg;    // a set of 16 general-purpose registers
  uint64_t rip;    // save current instruction address register
  uint8_t  codes;  // condition codes
} core_t;

core_t core;

#endif  // ASSEMBLER_CPU_H