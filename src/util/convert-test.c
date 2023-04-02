#include "assembler/convert.h"

#include "test/test.h"

TEST_BEGIN(test_str_to_uint64) {
  char num_str_list[][256] = {
    "0",
    "-0",
    "0x0",
    "1234",
    "0x1234",
    "0xabcd",
    "-0xabcd",
    "-1234",
    "2147483647",
    "-2147483648",
    "0x8000000000000000",
    "0xffffffffffffffff",
    "0177",
    "-0177"
  };

  EXPECT_U64_EQ(str2uint64(num_str_list[0]), 0);
  EXPECT_U64_EQ(str2uint64(num_str_list[1]), 0);
  EXPECT_U64_EQ(str2uint64(num_str_list[2]), 0);
  EXPECT_U64_EQ(str2uint64(num_str_list[3]), 1234);
  EXPECT_U64_EQ(str2uint64(num_str_list[4]), 0x1234);
  EXPECT_U64_EQ(str2uint64(num_str_list[5]), 0xabcd);
  EXPECT_U64_EQ(str2uint64(num_str_list[6]), -0xabcd);
  EXPECT_U64_EQ(str2uint64(num_str_list[7]), -1234);
  EXPECT_U64_EQ(str2uint64(num_str_list[8]), 2147483647);
  EXPECT_U64_EQ(str2uint64(num_str_list[9]), -2147483648);
  EXPECT_U64_EQ(str2uint64(num_str_list[10]), 0x8000000000000000);
  EXPECT_U64_EQ(str2uint64(num_str_list[11]), 0xffffffffffffffff);
  EXPECT_U64_EQ(str2uint64(num_str_list[12]), 0177);
  EXPECT_U64_EQ(str2uint64(num_str_list[13]), -0177);
}
TEST_END

int main(void) {
  return RUN_TESTS(test_str_to_uint64);
}