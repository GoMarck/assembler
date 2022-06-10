#pragma once

#ifndef ASSEMBLER_TEST_TEST_H
#define ASSEMBLER_TEST_TEST_H

#include "test_internal.h"

#define EXPECT_TRUE(a)  EXPECT_EQ(a, true)
#define EXPECT_FALSE(a) EXPECT_EQ(a, false)

#define ASSERT_TRUE(a)  VERIFY_EQ(a, true)
#define ASSERT_FALSE(a) VERIFY_EQ(a, false)

#define EXPECT_STR_EQ(a, b) VERIFY_STR_EQ(false, a, b)
#define EXPECT_STR_NE(a, b) VERIFY_STR_NE(false, a, b)

#define ASSERT_STR_EQ(a, b) VERIFY_STR_EQ(true, a, b)
#define ASSERT_STR_NE(a, b) VERIFY_STR_NE(true, a, b)

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

#define EXPECT_PTR_EQ(a, b) EXPECT_CMP(void *, a, b, ==, !=, "p")
#define EXPECT_PTR_NE(a, b) EXPECT_CMP(void *, a, b, !=, ==, "p")
#define EXPECT_PTR_NULL(a) EXPECT_CMP(void *, a, NULL, ==, !=, "p")
#define EXPECT_PTR_NOT_NULL(a) EXPECT_CMP(void *, a, NULL, !=, ==, "p")

#define ASSERT_PTR_EQ(a, b) ASSERT_CMP(void *, a, b, ==, !=, "p")
#define ASSERT_PTR_NE(a, b) ASSERT_CMP(void *, a, b, !=, ==, "p")
#define ASSERT_PTR_NULL(a) ASSERT_CMP(void *, a, NULL, ==, !=, "p")
#define ASSERT_PTR_NOT_NULL(a) ASSERT_CMP(void *, a, NULL, !=, ==, "p")

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

#endif