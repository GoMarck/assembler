#include <stdio.h>

#include "memory/inst.h"

#define INST_LEN 20

extern inst_t program[INST_LEN];

void print_inst()
{
    for (int i = 0; i < INST_LEN; ++i) {
        printf("%s\n", program[i].dsp);
    }
}

int main()
{
    print_inst();
    return 0;
}