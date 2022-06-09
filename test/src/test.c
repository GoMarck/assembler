#include "test/test.h"

#include <stdio.h>

static unsigned int test_count = 0;
static test_status_t test_counts[3] = {0, 0, 0};
static test_status_t test_status = test_status_pass;
static const char *test_name = "";

static const char *test_status_str(test_status_t status) {
  switch (status) {
    case test_status_pass:
      return "pass";
    case test_status_skip:
      return "skip";
    case test_status_fail:
      return "fail";
    default:
      return "unknown";
  }
}

void test_print(const char *format, ...) {
  va_list ap;

  va_start(ap, format);
  // now we only print result to stderr, maybe we can add some color?
  fprintf(stderr, format, ap);
  va_end(ap);
}

void test_init(const char *name) {
  ++test_count;
  test_status = test_status_pass;
  test_name = name;
}

void test_finish() {
  ++test_counts[test_status];
  test_print("%s: %s\n", test_name, test_status_str(test_status));
}

test_status_t test_core(test_t *t, ...) {
  test_status_t ret = test_status_pass;
  va_list ap;

  va_start(ap, t);

  for (; t != NULL; t = va_arg(ap, test_t *)) {
    t();
    if (test_status > ret) {
      ret = test_status;
    }
  }

  test_print("--- %s: %u/%u, %s: %u/%u, %s: %u/%u ---\n",
                    test_status_str(test_status_pass),
                    test_counts[test_status_pass], test_count,
                    test_status_str(test_status_skip),
                    test_counts[test_status_skip], test_count,
                    test_status_str(test_status_fail),
                    test_counts[test_status_fail], test_count);

  va_end(ap);
  return ret;
}