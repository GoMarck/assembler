#pragma once

#ifndef ASSEMBLER_TEST_TEST_H
#define ASSEMBLER_TEST_TEST_H

#include "test_internal.h"

// Macros for testing condition, equalities and inequalities.
//
//    * {ASSERT|EXPECT}_TRUE(cond):        Tests that cond == true
//    * {ASSERT|EXPECT}_FALSE(cond):       Tests that cond == false
//    * {ASSERT|EXPECT}_{TYPE}_EQ(v1, v2): Tests that v1 == v2
//    * {ASSERT|EXPECT}_{TYPE}_NE(v1, v2): Tests that v1 != v2
//    * {ASSERT|EXPECT}_{TYPE}_GE(v1, v2): Tests that v1 >= v2
//    * {ASSERT|EXPECT}_{TYPE}_GT(v1, v2): Tests that v1 > v2
//    * {ASSERT|EXPECT}_{TYPE}_LE(v1, v2): Tests that v1 <= v2
//    * {ASSERT|EXPECT}_{TYPE}_LT(v1, v2): Tests that v1 < v2
//    * {ASSERT|EXPECT}_PTR_NULL(P):       Tests that P == NULL
//    * {ASSERT|EXPECT}_PTR_NOT_NULL(P):   Tests that P != NULL
//
// When they are not, test framework prints both the tested
// expression and their actual value.
//
// Marco ASSERT_??() would abort the testcases when meets fail,
// but EXPECT_??() would continue to run the testcases even it 
// meets fail.
//
// Examples:
//
//   EXPECT_TRUE(Foo())
//   EXPECT_D32_LT(i, size)
//   EXPECT_PTR_NULL(ptr)

#define EXPECT_TRUE(cond)  EXPECT_EQ(cond, true)
#define EXPECT_FALSE(cond) EXPECT_EQ(cond, false)

#define ASSERT_TRUE(cond)  ASSERT_EQ(cond, true)
#define ASSERT_FALSE(cond) ASSERT_EQ(cond, false)

#define EXPECT_STR_EQ(v1, v2) VERIFY_STR_EQ(false, "Expection", v1, v2)
#define EXPECT_STR_NE(v1, v2) VERIFY_STR_NE(false, "Expection", v1, v2)

#define ASSERT_STR_EQ(v1, v2) VERIFY_STR_EQ(true, "Assertion", v1, v2)
#define ASSERT_STR_NE(v1, v2) VERIFY_STR_NE(true, "Assertion", v1, v2)

#define EXPECT_U32_EQ(v1, v2) EXPECT_CMP(uint32_t, v1, v2, ==, !=, "ld")
#define EXPECT_U32_NE(v1, v2) EXPECT_CMP(uint32_t, v1, v2, !=, ==, "ld")
#define EXPECT_U32_GE(v1, v2) EXPECT_CMP(uint32_t, v1, v2, >=, <,  "ld")
#define EXPECT_U32_GT(v1, v2) EXPECT_CMP(uint32_t, v1, v2, >,  <=, "ld")
#define EXPECT_U32_LE(v1, v2) EXPECT_CMP(uint32_t, v1, v2, <=, >,  "ld")
#define EXPECT_U32_LT(v1, v2) EXPECT_CMP(uint32_t, v1, v2, <,  >=, "ld")

#define EXPECT_D32_EQ(v1, v2) EXPECT_CMP(int32_t,  v1, v2, ==, !=, "d")
#define EXPECT_D32_NE(v1, v2) EXPECT_CMP(int32_t,  v1, v2, !=, ==, "d")
#define EXPECT_D32_GE(v1, v2) EXPECT_CMP(int32_t,  v1, v2, >=, <,  "d")
#define EXPECT_D32_GT(v1, v2) EXPECT_CMP(int32_t,  v1, v2, >,  <=, "d")
#define EXPECT_D32_LE(v1, v2) EXPECT_CMP(int32_t,  v1, v2, <=, >,  "d")
#define EXPECT_D32_LT(v1, v2) EXPECT_CMP(int32_t,  v1, v2, <,  >=, "d")

#define EXPECT_U64_EQ(v1, v2) EXPECT_CMP(uint64_t, v1, v2, ==, !=, "lu")
#define EXPECT_U64_NE(v1, v2) EXPECT_CMP(uint64_t, v1, v2, !=, ==, "lu")
#define EXPECT_U64_GE(v1, v2) EXPECT_CMP(uint64_t, v1, v2, >=, <,  "lu")
#define EXPECT_U64_GT(v1, v2) EXPECT_CMP(uint64_t, v1, v2, >,  <=, "lu")
#define EXPECT_U64_LE(v1, v2) EXPECT_CMP(uint64_t, v1, v2, <=, >,  "lu")
#define EXPECT_U64_LT(v1, v2) EXPECT_CMP(uint64_t, v1, v2, <,  >=, "lu")

#define EXPECT_D64_EQ(v1, v2) EXPECT_CMP(int64_t,  v1, v2, ==, !=, "u")
#define EXPECT_D64_NE(v1, v2) EXPECT_CMP(int64_t,  v1, v2, !=, ==, "u")
#define EXPECT_D64_GE(v1, v2) EXPECT_CMP(int64_t,  v1, v2, >=, <,  "u")
#define EXPECT_D64_GT(v1, v2) EXPECT_CMP(int64_t,  v1, v2, >,  <=, "u")
#define EXPECT_D64_LE(v1, v2) EXPECT_CMP(int64_t,  v1, v2, <=, >,  "u")
#define EXPECT_D64_LT(v1, v2) EXPECT_CMP(int64_t,  v1, v2, <,  >=, "u")

#define ASSERT_U32_EQ(v1, v2) ASSERT_CMP(uint32_t, v1, v2, ==, !=, "ld")
#define ASSERT_U32_NE(v1, v2) ASSERT_CMP(uint32_t, v1, v2, !=, ==, "ld")
#define ASSERT_U32_GE(v1, v2) ASSERT_CMP(uint32_t, v1, v2, >=, <,  "ld")
#define ASSERT_U32_GT(v1, v2) ASSERT_CMP(uint32_t, v1, v2, >,  <=, "ld")
#define ASSERT_U32_LE(v1, v2) ASSERT_CMP(uint32_t, v1, v2, <=, >,  "ld")
#define ASSERT_U32_LT(v1, v2) ASSERT_CMP(uint32_t, v1, v2, <,  >=, "ld")

#define ASSERT_D32_EQ(v1, v2) ASSERT_CMP(int32_t,  v1, v2, ==, !=, "d")
#define ASSERT_D32_NE(v1, v2) ASSERT_CMP(int32_t,  v1, v2, !=, ==, "d")
#define ASSERT_D32_GE(v1, v2) ASSERT_CMP(int32_t,  v1, v2, >=, <,  "d")
#define ASSERT_D32_GT(v1, v2) ASSERT_CMP(int32_t,  v1, v2, >,  <=, "d")
#define ASSERT_D32_LE(v1, v2) ASSERT_CMP(int32_t,  v1, v2, <=, >,  "d")
#define ASSERT_D32_LT(v1, v2) ASSERT_CMP(int32_t,  v1, v2, <,  >=, "d")

#define ASSERT_U64_EQ(v1, v2) ASSERT_CMP(uint64_t, v1, v2, ==, !=, "lu")
#define ASSERT_U64_NE(v1, v2) ASSERT_CMP(uint64_t, v1, v2, !=, ==, "lu")
#define ASSERT_U64_GE(v1, v2) ASSERT_CMP(uint64_t, v1, v2, >=, <,  "lu")
#define ASSERT_U64_GT(v1, v2) ASSERT_CMP(uint64_t, v1, v2, >,  <=, "lu")
#define ASSERT_U64_LE(v1, v2) ASSERT_CMP(uint64_t, v1, v2, <=, >,  "lu")
#define ASSERT_U64_LT(v1, v2) ASSERT_CMP(uint64_t, v1, v2, <,  >=, "lu")

#define ASSERT_D64_EQ(v1, v2) ASSERT_CMP(int64_t,  v1, v2, ==, !=, "u")
#define ASSERT_D64_NE(v1, v2) ASSERT_CMP(int64_t,  v1, v2, !=, ==, "u")
#define ASSERT_D64_GE(v1, v2) ASSERT_CMP(int64_t,  v1, v2, >=, <,  "u")
#define ASSERT_D64_GT(v1, v2) ASSERT_CMP(int64_t,  v1, v2, >,  <=, "u")
#define ASSERT_D64_LE(v1, v2) ASSERT_CMP(int64_t,  v1, v2, <=, >,  "u")
#define ASSERT_D64_LT(v1, v2) ASSERT_CMP(int64_t,  v1, v2, <,  >=, "u")

#define EXPECT_PTR_EQ(p1, p2) EXPECT_CMP(void *, p1, p2, ==, !=, "p")
#define EXPECT_PTR_NE(p1, p2) EXPECT_CMP(void *, p1, p2, !=, ==, "p")
#define EXPECT_PTR_NULL(p) EXPECT_CMP(void *, p, NULL, ==, !=, "p")
#define EXPECT_PTR_NOT_NULL(p) EXPECT_CMP(void *, p, NULL, !=, ==, "p")

#define ASSERT_PTR_EQ(p1, p2) ASSERT_CMP(void *, p1, p2, ==, !=, "p")
#define ASSERT_PTR_NE(p1, p2) ASSERT_CMP(void *, p1, p2, !=, ==, "p")
#define ASSERT_PTR_NULL(p) ASSERT_CMP(void *, p, NULL, ==, !=, "p")
#define ASSERT_PTR_NOT_NULL(p) ASSERT_CMP(void *, p, NULL, !=, ==, "p")

// Macros for testcases construct.
// 
// Examples:
//
//   TEST_BEGIN(test_name) {               <-- test begin to start a new testcase with test_name
//     ...
//     EXPECT_EQ(i, j);                    <-- test body contains test codes and test marco
//   }
//   TEST_END                              <-- test end to mark testcase done
//  
//   int main(void) {
//     return RUN_TESTS(test_euqal, xxx);  <-- run testcases with its test_name
//   }

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