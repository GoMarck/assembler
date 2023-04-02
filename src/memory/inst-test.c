#include "assembler/inst.h"

#include "test/test.h"
#include <bits/stdint-uintn.h>
#include <stdio.h>

TEST_BEGIN(test_parse_operand_immediate_number) {
  char *imm_operand_list[] = {
    "$0",
    "$-0",
    "$0x0",
    "$1234",
    "$0x1234",
    "$0xabcd",
    "$-0xabcd",
    "$-1234",
    "$2147483647",
    "$-2147483648",
    "$0x8000000000000000",
    "$0xffffffffffffffff",
    "$0177",
    "$-0177"
  };

  uint64_t cmp_list[] = {
    0, 0, 0, 1234, 0x1234, 0xabcd, -0xabcd,
    -1234, 2147483647, -2147483648,
    0x8000000000000000, 0xffffffffffffffff,
    0177, -0177
  };

  for (size_t i = 0; i < 14; ++i) {
    operand_t op;
    parse_operand(imm_operand_list[i], &op);
    EXPECT_U32_EQ(op.type, IMM);
    EXPECT_U64_EQ(op.imm, cmp_list[i]);
  }
}
TEST_END

int main(void) { return RUN_TESTS(test_parse_operand_immediate_number); }