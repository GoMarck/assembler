#pragma once

#ifndef ASSEMBLER_UTIL_LOG_H
#define ASSEMBLER_UTIL_LOG_H

#include <stdio.h>

typedef enum LogLevel {
  VERBOSE = 0,
  INFO = 1,
  WARNING = 2,
  ERROR = 3,
  FATAL = 4
} LogLevel;

#define MAX_LOG_MESSAGE_LEN 30000

// Now we don't have the multi-thread requirement,
// so the thread local optimize is not required.
char message_text[MAX_LOG_MESSAGE_LEN + 1];

#define LOG_LEVEL INFO

// TODO: need complete.
#define LOG_TO_STDERR(level, ...) \
  do {                            \
    if (level < LOG_LEVEL) {      \
      return;                     \
    }                             \
  } while (0)

#define STDERR_LOG_VERBOSE(...) log_to(VERBOSE, __VA_ARGS__)

#define STDERR_LOG_INFO(...) log_to(INFO, __VA_ARGS__)

#define STDERR_LOG_WARNING(...) log_to(WARNING, __VA_ARGS__)

#define STDERR_LOG_ERROR(...) log_to(ERROR, __VA_ARGS__)

#define STDERR_LOG_FATAL(...) log_to(FATAL, __VA_ARGS__)

#define LOG(level, ...) STDERR_LOG_##level(__VA_ARGS__)

#endif  // ASSEMBLER_UTIL_LOG_H