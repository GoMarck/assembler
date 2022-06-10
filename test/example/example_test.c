#include <stdint.h>

#include "test/test.h"

TEST_BEGIN(test_assert_equal) {
  uint64_t a = UINT64_MAX;
  int64_t b = -1;
  ASSERT_U64_EQ(a, b);
}
TEST_END

int main() {
  // run testcases defined in this file.
  return RUN_TESTS(test_assert_equal);
}