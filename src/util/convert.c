#include "assembler/convert.h"

#include <bits/stdint-uintn.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler/log.h"

uint64_t str2uint64(const char *str){
  return str2uint64_range(str, 0, strlen(str));
}

typedef enum num_type_t {
  DECIMAL = 0,
  OCTAL = 1,
  HEX = 2
} num_type_t;

uint64_t
str2uint64_range(const char *str, size_t beg, size_t end) {
  // We may meets the number string shown as follow:
  // - 1. (-)1234:   number expressed in decimal
  // - 2. (-)01234:  number expressed in octal
  // - 3. (-)0x1234: number expressed in hexadecimal
  bool negative = false;
  uint64_t ret = 0;
  uint32_t state = 0;
  for (size_t i = beg; i < end; ++i) {
    if (state == 0) {
      if (str[i] == '-') {
        negative = true;
        state = 0;
      } else if (str[i] == '0') {
        state = 1;
      } else if (str[i] >= '1' && str[i] <= '9') {
        state = 5;
        ret = ret * 10 + (str[i] - '0');
      } else {
        goto fail_handle;
      }
    } else if (state == 1) {
      if (str[i] == 'x' || str[i] == 'X') {
        state = 2;
      } else if (str[i] >= '0' && str[i] <= '9') {
        state = 3;
        ret = ret * 8 + (str[i] - '0');
      } else {
        goto fail_handle;
      }
    } else if (state == 2 || state == 4) {
      if (str[i] >= '0' && str[i] <= '9') {
        ret = ret * 16 + (str[i] - '0');
      } else if (str[i] >= 'a' && str[i] <= 'f') {
        ret = ret * 16 + (str[i] - 'a') + 10;
      } else if (str[i] >= 'A' && str[i] <= 'F') {
        ret = ret * 16 + (str[i] - 'A') + 10;
      } else {
        goto fail_handle;
      }
      state = 4;
    } else if (state == 3 && str[i] >= '0' && str[i] <= '9') {
      ret = ret * 8 + (str[i] - '0');
    } else if (state == 5 && str[i] >= '0' && str[i] <= '9') {
      ret = ret * 10 + (str[i] - '0');
    } else {
      goto fail_handle;
    }
  }
  return negative ? -ret : ret;

fail_handle:
  printf("Parse string to number failed: %s\n", str);
  exit(1);
}