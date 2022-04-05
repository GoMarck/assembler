# Assembler
## What is Assembler
Assembler is a very simple emulator, just for fun. Its implementation theory is derived from the book CSAPP (CMU 15-213).

The project is reference from: [yangminz
/bcst_csapp](https://github.com/yangminz/bcst_csapp). Thanks to [yangminz](https://github.com/yangminz), I learn a lot from the project and his lesson in [bilibili](https://space.bilibili.com/4564101/video).

## What I achieved
I have successfully implement an assembler emulator to parse a very simple C codes:
```c
#include <stdint.h>

int64_t add(int64_t a, int64_t b)
{
    int64_t c = a + b;
    return c;
}

int64_t main()
{
    int64_t a = 0x12340000;
    int64_t b = 0xabcd;
    int64_t c = add(a, b);
    return 0;
}
```
Its assembly instructions are show as follow:
```asm
0000000000001129 <add>:
    112d:       55                      push   %rbp
    112e:       48 89 e5                mov    %rsp,%rbp
    1131:       48 89 7d e8             mov    %rdi,-0x18(%rbp)
    1135:       48 89 75 e0             mov    %rsi,-0x20(%rbp)
    1139:       48 8b 55 e8             mov    -0x18(%rbp),%rdx
    113d:       48 8b 45 e0             mov    -0x20(%rbp),%rax
    1141:       48 01 d0                add    %rdx,%rax
    1144:       48 89 45 f8             mov    %rax,-0x8(%rbp)
    1148:       48 8b 45 f8             mov    -0x8(%rbp),%rax
    114c:       5d                      pop    %rbp
    114d:       c3                      retq

000000000000114e <main>:
    115a:       48 c7 45 e8 00 00 34    movq   $0x12340000,-0x18(%rbp)
    1161:       12
    1162:       48 c7 45 f0 cd ab 00    movq   $0xabcd,-0x10(%rbp)
    1169:       00
    116a:       48 8b 55 f0             mov    -0x10(%rbp),%rdx
    116e:       48 8b 45 e8             mov    -0x18(%rbp),%rax
    1172:       48 89 d6                mov    %rdx,%rsi
    1175:       48 89 c7                mov    %rax,%rdi
    1178:       e8 ac ff ff ff          callq  1129 <add>
    117d:       48 89 45 f8             mov    %rax,-0x8(%rbp)
    1181:       b8 00 00 00 00          mov    $0x0,%eax
```
I manually parsed its assembly instructions and simply simulated the state of its CPU and memory. And it has been success.
## TODO List
Based on the features we have implemented, we want to implement these features in the future:
- Find a favorite C programming specification and follow it.
- We need a story folder to place some design documents.
- Refactor the project to make the hierarchy more reasonable.
- Implement an instruction set string parsing state machine.
- Implement SRAM cache function.
- Implement MMU(Memory Manager Unit) function.
- Implement the linking function.

Our next target is implement the codes as follow:
```c
#include <stdint.h>

uint64_t sum(uint64_t n)
{
    if (n == 0) {
        return 0;
    } else {
        return n + sum(n - 1);
    }
}

int main()
{
    uint64_t a = sum(3);
    return 0;
}
```
