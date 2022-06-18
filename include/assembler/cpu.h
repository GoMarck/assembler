#pragma once

#ifndef ASSEMBLER_CPU_H
#define ASSEMBLER_CPU_H

#include <stdbool.h>
#include <stdint.h>

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

typedef struct Register {
  union {
    uint64_t rax;
    uint32_t eax;
    uint16_t ax;
    uint8_t  al;
  };

  union {
    uint64_t rbx;
    uint32_t ebx;
    uint16_t bx;
    uint8_t  bl;
  };

  union {
    uint64_t rcx;
    uint32_t ecx;
    uint16_t cx;
    uint8_t  cl;
  };

  union {
    uint64_t rsi;
    uint32_t esi;
    uint16_t si;
    uint8_t  sil;
  };

  union {
    uint64_t rdi;
    uint32_t edi;
    uint16_t di;
    uint8_t  dil;
  };

  union {
    uint64_t rbp;
    uint32_t ebp;
    uint16_t bp;
    uint8_t  bpl;
  };

  union {
    uint64_t rsp;
    uint32_t esp;
    uint16_t sp;
    uint8_t  spl;
  };

  union {
    uint64_t r8;
    uint32_t r8d;
    uint16_t r8w;
    uint8_t  r8b;
  };

  union {
    uint64_t r9;
    uint32_t r9d;
    uint16_t r9w;
    uint8_t  r9b;
  };

  union {
    uint64_t r10;
    uint32_t r10d;
    uint16_t r10w;
    uint8_t  r10b;
  };

  union {
    uint64_t r11;
    uint32_t r11d;
    uint16_t r11w;
    uint8_t  r11b;
  };

  union {
    uint64_t r12;
    uint32_t r12d;
    uint16_t r12w;
    uint8_t  r12b;
  };

  union {
    uint64_t r13;
    uint32_t r13d;
    uint16_t r13w;
    uint8_t  r13b;
  };

  union {
    uint64_t r14;
    uint32_t r14d;
    uint16_t r14w;
    uint8_t  r14b;
  };

  union {
    uint64_t r15;
    uint32_t r15d;
    uint16_t r15w;
    uint8_t  r15b;
  };
} Register;

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

typedef struct Core {
  Register reg;    // a set of 16 general-purpose registers
  uint64_t rip;    // save current instruction address register
  uint8_t  codes;  // condition codes
} Core;

Core core;

#endif  // ASSEMBLER_CPU_H