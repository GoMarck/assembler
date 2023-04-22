# Assembler
## What is Assembler
Assembler is a very simple emulator, just for fun. Its implementation theory is derived from the book CSAPP (CMU 15-213).

The project is reference from: [yangminz
/bcst_csapp](https://github.com/yangminz/bcst_csapp). Thanks to [yangminz](https://github.com/yangminz), I learn a lot from the project and his lesson in [bilibili](https://space.bilibili.com/4564101/video). And we obey the [Google C++ style guide](https://google.github.io/styleguide/cppguide.html).

## What I achieved
I have successfully implement an assembler emulator to parse two very simple C codes:
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
I manually parsed its assembly instructions and simply simulated the state of its CPU and memory. And it run success and verified.

Another C codes is shown as below:
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
Its assembly instructions are show as follow:
```asm
0000000000001129 <sum>:
    1129:       f3 0f 1e fa             endbr64 
    112d:       55                      push   %rbp
    112e:       48 89 e5                mov    %rsp,%rbp
    1131:       48 83 ec 10             sub    $0x10,%rsp
    1135:       48 89 7d f8             mov    %rdi,-0x8(%rbp)
    1139:       48 83 7d f8 00          cmpq   $0x0,-0x8(%rbp)
    113e:       75 07                   jne    1147 <sum+0x1e>
    1140:       b8 00 00 00 00          mov    $0x0,%eax
    1145:       eb 17                   jmp    115e <sum+0x35>
    1147:       48 8b 45 f8             mov    -0x8(%rbp),%rax
    114b:       48 83 e8 01             sub    $0x1,%rax
    114f:       48 89 c7                mov    %rax,%rdi
    1152:       e8 d2 ff ff ff          callq  1129 <sum>
    1157:       48 8b 55 f8             mov    -0x8(%rbp),%rdx
    115b:       48 01 d0                add    %rdx,%rax
    115e:       c9                      leaveq 
    115f:       c3                      retq   

0000000000001160 <main>:
    1160:       f3 0f 1e fa             endbr64 
    1164:       55                      push   %rbp
    1165:       48 89 e5                mov    %rsp,%rbp
    1168:       48 83 ec 10             sub    $0x10,%rsp
    116c:       bf 03 00 00 00          mov    $0x3,%edi
    1171:       e8 b3 ff ff ff          callq  1129 <sum>
    1176:       48 89 45 f8             mov    %rax,-0x8(%rbp)
    117a:       b8 00 00 00 00          mov    $0x0,%eax
    117f:       c9                      leaveq 
    1180:       c3                      retq   
```
I have successfully simulated the call of a recursive function. Compared with the add function, it has more jump instructions and conditional code implementations, and it run success and verified too.

## TODO List
Based on the features we have implemented, we want to implement these features in the future:
- Implement the linking function.
- Improve the design documentation.
- Provide a more efficient test framework, ST testing currently takes too long.
- Optimize instruction set string parsing algorithm, which is now very inefficient.
- Implement SRAM cache function.
- Implement MMU(Memory Manager Unit) function.

## Some useful link
- [Google C++ style guide [EN]](https://google.github.io/styleguide/cppguide.html)
- [Google C++ style guide [CN]](https://zh-google-styleguide.readthedocs.io/en/latest/)
- [CMU CSAPP Lessons]()
- [Assembler Lessons](https://space.bilibili.com/4564101/video)
