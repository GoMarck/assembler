#pragma once

#ifndef ASSEMBLER_TEST_TEST_H
#define ASSEMBLER_TEST_TEST_H

#include <stdbool.h>
#include <stdint.h>

#define EXPECT_TRUE(a)  VERIFY_TRUE(false, a)
#define EXPECT_FALSE(a) VERIFY_FALSE(false, a)

#define ASSERT_TRUE(a) VERIFY_TRUE(true, a)
#define ASSERT_FALSE(a) VERIFY_FALSE(true, a)

#define EXPECT_STR_EQ(a, b) VERIFY_STR_EQ(false, a, b)
#define EXPECT_STR_NE(a, b) VERIFY_STR_NE(false, a, b)

#define ASSERT_STR_EQ(a, b) VERIFY_STR_EQ(true, a, b)
#define ASSERT_STR_NE(a, b) VERIFY_STR_NE(true, a, b)

#define VERIFY_CMP(assert, str, t, a, b, cmp, cmp_ne, pf)     \
  do {                                                        \
    const t a_ = (t)(a);                                      \
    const t b_ = (t)(b);                                      \
    if (!(a_ cmp b_)) {                                       \
      test_print(                                             \
          "%s:%s:%d: Failed %s: "                             \
          "(%s) " #cmp " (%s) --> "                           \
          "%" pf " " #cmp_ne "%" pf "\n",                     \
          __func__, __FILE__, __LINE__, str, #a, #b, a_, b_); \
      mark_test_fail();                                       \
      if (assert) {                                           \
        goto label_test_end;                                  \
      }                                                       \
    }                                                         \
  } while (0)

#define EXPECT_CMP(t, a, b, cmp, cmp_ne, pf) \
  VERIFY_CMP(false, "Expection", t, a, b, cmq, cmq_ne, pf)

#define ASSERT_CMP(t, a, b, cmp, cmp_ne, pf) \
  VERIFY_CMP(true, "Assertion", t, a, b, cmq, cmq_ne, pf)

#define EXPECT_U32_EQ(a, b) EXPECT_CMP(uint32_t, a, b, ==, !=, "ld")
#define EXPECT_U32_NE(a, b) EXPECT_CMP(uint32_t, a, b, !=, ==, "ld")
#define EXPECT_U32_GE(a, b) EXPECT_CMP(uint32_t, a, b, >=, <,  "ld")
#define EXPECT_U32_GT(a, b) EXPECT_CMP(uint32_t, a, b, >,  <=, "ld")
#define EXPECT_U32_LE(a, b) EXPECT_CMP(uint32_t, a, b, <=, >,  "ld")
#define EXPECT_U32_LT(a, b) EXPECT_CMP(uint32_t, a, b, <,  >=, "ld")

#define EXPECT_D32_EQ(a, b) EXPECT_CMP(int32_t,  a, b, ==, !=, "d")
#define EXPECT_D32_NE(a, b) EXPECT_CMP(int32_t,  a, b, !=, ==, "d")
#define EXPECT_D32_GE(a, b) EXPECT_CMP(int32_t,  a, b, >=, <,  "d")
#define EXPECT_D32_GT(a, b) EXPECT_CMP(int32_t,  a, b, >,  <=, "d")
#define EXPECT_D32_LE(a, b) EXPECT_CMP(int32_t,  a, b, <=, >,  "d")
#define EXPECT_D32_LT(a, b) EXPECT_CMP(int32_t,  a, b, <,  >=, "d")

#define EXPECT_U64_EQ(a, b) EXPECT_CMP(uint64_t, a, b, ==, !=, "lu")
#define EXPECT_U64_NE(a, b) EXPECT_CMP(uint64_t, a, b, !=, ==, "lu")
#define EXPECT_U64_GE(a, b) EXPECT_CMP(uint64_t, a, b, >=, <,  "lu")
#define EXPECT_U64_GT(a, b) EXPECT_CMP(uint64_t, a, b, >,  <=, "lu")
#define EXPECT_U64_LE(a, b) EXPECT_CMP(uint64_t, a, b, <=, >,  "lu")
#define EXPECT_U64_LT(a, b) EXPECT_CMP(uint64_t, a, b, <,  >=, "lu")

#define EXPECT_D64_EQ(a, b) EXPECT_CMP(int64_t,  a, b, ==, !=, "u")
#define EXPECT_D64_NE(a, b) EXPECT_CMP(int64_t,  a, b, !=, ==, "u")
#define EXPECT_D64_GE(a, b) EXPECT_CMP(int64_t,  a, b, >=, <,  "u")
#define EXPECT_D64_GT(a, b) EXPECT_CMP(int64_t,  a, b, >,  <=, "u")
#define EXPECT_D64_LE(a, b) EXPECT_CMP(int64_t,  a, b, <=, >,  "u")
#define EXPECT_D64_LT(a, b) EXPECT_CMP(int64_t,  a, b, <,  >=, "u")

#define EXPECT_PTR_EQ(a, b) EXPECT_CMP(void *, a, b, ==, !=, "p")
#define EXPECT_PTR_NE(a, b) EXPECT_CMP(void *, a, b, !=, ==, "p")
#define EXPECT_PTR_NULL(a) EXPECT_CMP(void *, a, NULL, ==, !=, "p")
#define EXPECT_PTR_NOT_NULL(a) EXPECT_CMP(void *, a, NULL, !=, ==, "p")

#define ASSERT_U32_EQ(a, b) ASSERT_CMP(uint32_t, a, b, ==, !=, "ld")
#define ASSERT_U32_NE(a, b) ASSERT_CMP(uint32_t, a, b, !=, ==, "ld")
#define ASSERT_U32_GE(a, b) ASSERT_CMP(uint32_t, a, b, >=, <,  "ld")
#define ASSERT_U32_GT(a, b) ASSERT_CMP(uint32_t, a, b, >,  <=, "ld")
#define ASSERT_U32_LE(a, b) ASSERT_CMP(uint32_t, a, b, <=, >,  "ld")
#define ASSERT_U32_LT(a, b) ASSERT_CMP(uint32_t, a, b, <,  >=, "ld")

#define ASSERT_D32_EQ(a, b) ASSERT_CMP(int32_t,  a, b, ==, !=, "d")
#define ASSERT_D32_NE(a, b) ASSERT_CMP(int32_t,  a, b, !=, ==, "d")
#define ASSERT_D32_GE(a, b) ASSERT_CMP(int32_t,  a, b, >=, <,  "d")
#define ASSERT_D32_GT(a, b) ASSERT_CMP(int32_t,  a, b, >,  <=, "d")
#define ASSERT_D32_LE(a, b) ASSERT_CMP(int32_t,  a, b, <=, >,  "d")
#define ASSERT_D32_LT(a, b) ASSERT_CMP(int32_t,  a, b, <,  >=, "d")

#define ASSERT_U64_EQ(a, b) ASSERT_CMP(uint64_t, a, b, ==, !=, "lu")
#define ASSERT_U64_NE(a, b) ASSERT_CMP(uint64_t, a, b, !=, ==, "lu")
#define ASSERT_U64_GE(a, b) ASSERT_CMP(uint64_t, a, b, >=, <,  "lu")
#define ASSERT_U64_GT(a, b) ASSERT_CMP(uint64_t, a, b, >,  <=, "lu")
#define ASSERT_U64_LE(a, b) ASSERT_CMP(uint64_t, a, b, <=, >,  "lu")
#define ASSERT_U64_LT(a, b) ASSERT_CMP(uint64_t, a, b, <,  >=, "lu")

#define ASSERT_D64_EQ(a, b) ASSERT_CMP(int64_t,  a, b, ==, !=, "u")
#define ASSERT_D64_NE(a, b) ASSERT_CMP(int64_t,  a, b, !=, ==, "u")
#define ASSERT_D64_GE(a, b) ASSERT_CMP(int64_t,  a, b, >=, <,  "u")
#define ASSERT_D64_GT(a, b) ASSERT_CMP(int64_t,  a, b, >,  <=, "u")
#define ASSERT_D64_LE(a, b) ASSERT_CMP(int64_t,  a, b, <=, >,  "u")
#define ASSERT_D64_LT(a, b) ASSERT_CMP(int64_t,  a, b, <,  >=, "u")

#define ASSERT_PTR_EQ(a, b) ASSERT_CMP(void *, a, b, ==, !=, "p")
#define ASSERT_PTR_NE(a, b) ASSERT_CMP(void *, a, b, !=, ==, "p")
#define ASSERT_PTR_NULL(a) ASSERT_CMP(void *, a, NULL, ==, !=, "p")
#define ASSERT_PTR_NOT_NULL(a) ASSERT_CMP(void *, a, NULL, !=, ==, "p")

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
    test_core(__VA_ARGS__, NULL)

void test_init(const char *name);
void test_finish();
void test_print(const char *format, ...);
test_status_t test_core(test_t *t);

#endif