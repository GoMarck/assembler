#include <stdint.h>

#include "test/test.h"

TEST_BEGIN(test_example) {
  int a = 0;
  int b = 1;
  int c = 2;
  int d = 0;
  ASSERT_D32_EQ(a, d);
  ASSERT_D32_NE(a, b);
  ASSERT_D32_GE(b, a);
  ASSERT_D32_GT(c, b);
  ASSERT_D32_LE(a, b);
  ASSERT_D32_LT(a, b);

  int *ap = &a;
  int *bp = &b;
  int *cp = NULL;
  ASSERT_PTR_EQ(ap, ap);
  ASSERT_PTR_NE(ap, bp);
  ASSERT_PTR_NULL(cp);
  ASSERT_PTR_NOT_NULL(ap);

  char *str1 = "Hello World!";
  char *str2 = "Hello World!";
  char *str3 = "Come on boy!";
  ASSERT_STR_EQ(str1, str2);
  ASSERT_STR_NE(str1, str3);

  ASSERT_TRUE(1 != 0);
  ASSERT_FALSE(1 == 0);
}
TEST_END

int main() {
  // run testcases defined in this file.
  return RUN_TESTS(test_example);
}