#include "cpu.h"

#include "test/test.h"

TEST_BEGIN(test_gp_register_bit) {
  Core *cr = &core;
  cr->reg.rax = 0x7766554433221100;
  EXPECT_U64_EQ(cr->reg.rax, 0x7766554433221100);
  EXPECT_U64_EQ(cr->reg.eax, 0x33221100);
  EXPECT_U64_EQ(cr->reg.ax, 0x1100);
  EXPECT_U64_EQ(cr->reg.al, 0x0);
}
TEST_END

int main(void) {
  return RUN_TESTS(test_register_bit); 
}