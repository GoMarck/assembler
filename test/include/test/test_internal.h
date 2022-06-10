#pragma once

#ifndef ASSEMBLER_TEST_TEST_INTERNAL_H
#define ASSEMBLER_TEST_TEST_INTERNAL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// clang-format off
#define VERIFY_CMP(assert, str, t, a, b, cmp, cmp_ne, pf)     \
  do {                                                        \
    const t a_ = (t)(a);                                      \
    const t b_ = (t)(b);                                      \
    if (!(a_ cmp b_)) {                                       \
      test_print("%s:%s:%d: Failed %s: "                      \
        "(%s) " #cmp " (%s) --> "                             \
        "%" pf " " #cmp_ne "%" pf "\n",                       \
        __func__, __FILE__, __LINE__,                         \
        str, #a, #b, a_, b_);                                 \
      test_fail();                                            \
      if (assert) {                                           \
        abort();                                              \
      }                                                       \
    }                                                         \
  } while (0)

#define EXPECT_CMP(t, a, b, cmp, cmp_ne, pf) \
  VERIFY_CMP(false, "Expection", t, a, b, cmp, cmp_ne, pf)

#define ASSERT_CMP(t, a, b, cmp, cmp_ne, pf) \
  VERIFY_CMP(true, "Assertion", t, a, b, cmp, cmp_ne, pf)

#define VERIFY_EQ(assert, str, a, b)                          \
  do {                                                        \
    const bool a_ = (bool)(a);                                \
    const bool b_ = (bool)(b);                                \
    if (a_ != b_) {                                           \
      test_print("%s:%s:%d: Failed %s: "                      \
        "(%s) != (%s) --> %s == %s\n",                        \
        __func__, __FILE__, __LINE__,                         \
        str, #a, #b, a_ ? "true" : "false",			          \
		b_ ? "true" : "false");                               \
      test_fail();                                            \
      if (assert) {                                           \
        abort();                                              \
      }                                                       \
    }                                                         \
  } while (0)
  
#define EXPECT_EQ(a, b) \
  VERIFY_EQ(false, "Expection", a, b)

#define ASSERT_EQ(a, b) \
  VERIFY_EQ(true, "Assertion", a, b)

#define VERIFY_STR_EQ(assert, str, a, b) \
  do {                                                        \
    if (strcmp((a), (b))) {                                   \
      test_print("%s:%s:%d: Failed %s: "                      \
        "(%s) != (%s) --> \"%s\" vs \"%s\"\n",                \
        __func__, __FILE__, __LINE__,                         \
        str, #a, #b, a, b);                                   \
      test_fail();                                            \
      if (assert) {                                           \
        abort();                                              \
      }                                                       \
    }                                                         \
  } while (0)

#define VERIFY_STR_NE(assert, str, a, b) \
  do {                                                        \
    if (!strcmp((a), (b))) {                                   \
      test_print("%s:%s:%d: Failed %s: "                      \
        "(%s) == (%s) --> \"%s\" vs \"%s\"\n",                \
        __func__, __FILE__, __LINE__,                         \
        str, #a, #b, a, b);                                   \
      test_fail();                                            \
      if (assert) {                                           \
        abort();                                              \
      }                                                       \
    }                                                         \
  } while (0)
  

typedef enum {
  test_status_pass = 0,
  test_status_skip = 1,
  test_status_fail = 2
} test_status_t;

typedef void(test_t)(void);

void test_init(const char *name);
void test_finish();
void test_fail();
void test_print(const char *format, ...);
test_status_t test_core(test_t *t, ...);

#endif