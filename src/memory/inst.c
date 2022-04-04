#include "memory/inst.h"

#include <stdio.h>

#define OP_SIZE 5

typedef void(*handler_t)(uint64_t, uint64_t);

handler_t handler_table[OP_SIZE];

static void move_handler(uint64_t src, uint64_t dst);
static void add_handler(uint64_t src, uint64_t dst);
static void call_handler(uint64_t src, uint64_t dst);
static void ret_handler(uint64_t src, uint64_t dst);
static void push_handler(uint64_t src, uint64_t dst);
static void pop_handler(uint64_t src, uint64_t dst);

// init instruction handler table.
void init_handler_table()
{
    handler_table[MOV_IMM_MEM] = move_handler;
    handler_table[CALL] = call_handler;
    handler_table[RET] = ret_handler;
    handler_table[PUSH] = push_handler;
    handler_table[POP] = pop_handler;
}

static uint64_t decode_od(od_t *od)
{
    uint64_t val = 0;
    if (od->type == IMM) {
        val = od->imm;
    } else if (od->type == REG) {
        val = (uint64_t)od->reg_b;
    } else {
        val += od->imm;
        if (od->reg_b != NULL) {
            val += *od->reg_b;
        }
        if (od->reg_i != NULL) {
            val += od->scal * *od->reg_i;
        }
    }
    return val;
}

// mov instruction implementation
static void move_handler(uint64_t src, uint64_t dst)
{
    printf("Unsupport implemenatation: mov\n");
}

// add instruction implementation
static void add_handler(uint64_t src, uint64_t dst)
{
    printf("Unsupport implemenatation: add\n");
}

// call instruction implementation
static void call_handler(uint64_t src, uint64_t dst)
{
    printf("Unsupport implemenatation: call\n");
}

// ret instruction implementation
static void ret_handler(uint64_t src, uint64_t dst)
{
    printf("Unsupport implemenatation: ret\n");
}

// call instruction implementation
static void push_handler(uint64_t src, uint64_t dst)
{
    printf("Unsupport implemenatation: push\n");
}

// ret instruction implementation
static void pop_handler(uint64_t src, uint64_t dst)
{
    printf("Unsupport implemenatation: pop\n");
}