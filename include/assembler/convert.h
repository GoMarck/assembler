#pragma once

#ifndef ASSEMBLER_CONVERT_H
#define ASSEMBLER_CONVERT_H

#include <stddef.h>
#include <stdint.h>

uint64_t str2uint64(const char *str);

uint64_t str2uint64_range(const char *str, size_t beg, size_t end);

#endif