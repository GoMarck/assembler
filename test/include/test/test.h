#pragma once

#ifndef ASSEMBLER_TEST_TEST_H
#define ASSEMBLER_TEST_TEST_H

#define EXPECT_STR_NE(a, b, ...)
#define EXPECT_STR_NE(a, b, ...)

#define EXPECT_U32_EQ(a, b, ...)
#define EXPECT_U32_NE(a, b, ...)
#define EXPECT_U32_GE(a, b, ...)
#define EXPECT_U32_GT(a, b, ...)
#define EXPECT_U32_LE(a, b, ...)
#define EXPECT_U32_LT(a, b, ...)

#define EXPECT_D32_EQ(a, b, ...)
#define EXPECT_D32_NE(a, b, ...)
#define EXPECT_D32_GE(a, b, ...)
#define EXPECT_D32_GT(a, b, ...)
#define EXPECT_D32_LE(a, b, ...)
#define EXPECT_D32_LT(a, b, ...)

#define EXPECT_U64_EQ(a, b, ...)
#define EXPECT_U64_NE(a, b, ...)
#define EXPECT_U64_GE(a, b, ...)
#define EXPECT_U64_GT(a, b, ...)
#define EXPECT_U64_LE(a, b, ...)
#define EXPECT_U64_LT(a, b, ...)

#define EXPECT_D64_EQ(a, b, ...)
#define EXPECT_D64_NE(a, b, ...)
#define EXPECT_D64_GE(a, b, ...)
#define EXPECT_D64_GT(a, b, ...)
#define EXPECT_D64_LE(a, b, ...)
#define EXPECT_D64_LT(a, b, ...)

#define EXPECT_PTR_EQ(a, b, ...)
#define EXPECT_PTR_NE(a, b, ...)
#define EXPECT_PTR_NULL(a, ...)
#define EXPECT_PTR_NOT_NULL(a, ...)

#define ASSERT_STR_EQ(a, b, ...)
#define ASSERT_STR_NE(a, b, ...)

#define ASSERT_U32_EQ(a, b, ...)
#define ASSERT_U32_NE(a, b, ...)
#define ASSERT_U32_GE(a, b, ...)
#define ASSERT_U32_GT(a, b, ...)
#define ASSERT_U32_LE(a, b, ...)
#define ASSERT_U32_LT(a, b, ...)

#define ASSERT_D32_EQ(a, b, ...)
#define ASSERT_D32_NE(a, b, ...)
#define ASSERT_D32_GE(a, b, ...)
#define ASSERT_D32_GT(a, b, ...)
#define ASSERT_D32_LE(a, b, ...)
#define ASSERT_D32_LT(a, b, ...)

#define ASSERT_U64_EQ(a, b, ...)
#define ASSERT_U64_NE(a, b, ...)
#define ASSERT_U64_GE(a, b, ...)
#define ASSERT_U64_GT(a, b, ...)
#define ASSERT_U64_LE(a, b, ...)
#define ASSERT_U64_LT(a, b, ...)

#define ASSERT_D64_EQ(a, b, ...)
#define ASSERT_D64_NE(a, b, ...)
#define ASSERT_D64_GE(a, b, ...)
#define ASSERT_D64_GT(a, b, ...)
#define ASSERT_D64_LE(a, b, ...)
#define ASSERT_D64_LT(a, b, ...)

#define ASSERT_PTR_EQ(a, b, ...)
#define ASSERT_PTR_NE(a, b, ...)
#define ASSERT_PTR_NULL(a, ...)
#define ASSERT_PTR_NOT_NULL(a, ...)

typedef enum {
  test_status_pass = 0,
  test_status_skip = 1,
  test_status_fail = 2
} test_status_t;

typedef void(test_t)(void);

#define TEST_BEGIN(f)   \
static void f(void) {   \
  test_init(#f);

#define TEST_END       \
  goto label_test_end; \
label_test_end:        \
  test_finish();       \
}

#define RUN_TESTS(...) \
    return test_core(__VA_ARGS__, NULL)

void test_init(const char *name);

void test_finish();

test_status_t test_core(test_t *t, ...);

#endif