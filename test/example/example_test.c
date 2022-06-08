#include "test/test.h"

TEST_BEGIN(test_assert_equal) {}
TEST_END

TEST_BEGIN(test_assert_not_equal) {}
TEST_END

int main() { return RUN_TESTS(test_assert_equal, test_assert_not_equal); }