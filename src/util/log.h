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

#define MAX_LOG_MESSAGE_PREFIX_LEN 1024
#define MAX_LOG_MESSAGE_LEN 30000

char message_prefix[MAX_LOG_MESSAGE_PREFIX_LEN + 1];

// Now we don't have the multi-thread requirement,
// so the thread local optimize is not required.
char message_text[MAX_LOG_MESSAGE_LEN + 1];

#define LOG_LEVEL INFO

#define LOG_TO_STDERR(level, ...)                                       \
  do {                                                                  \
    if (level < LOG_LEVEL) {                                            \
      return;                                                           \
    }                                                                   \
    log_to_stderr(message_prefix, sizeof(message_prefix), "%s %s:%d] ", \
                  level_to_str(level), __FILE__, __LINE__);             \
    log_to_stderr(message_text, sizeof(message_text), __VA_ARGS__);     \
    log_to_stderr("\n");                                                \
    if (level == FATAL) {                                               \
      abort();                                                          \
    }                                                                   \
  } while (0)

#define STDERR_LOG_VERBOSE(...) LOG_TO_STDERR(VERBOSE, __VA_ARGS__)

#define STDERR_LOG_INFO(...) LOG_TO_STDERR(INFO, __VA_ARGS__)

#define STDERR_LOG_WARNING(...) LOG_TO_STDERR(WARNING, __VA_ARGS__)

#define STDERR_LOG_ERROR(...) LOG_TO_STDERR(ERROR, __VA_ARGS__)

#define STDERR_LOG_FATAL(...) LOG_TO_STDERR(FATAL, __VA_ARGS__)

#define LOG(level, ...) STDERR_LOG_##level(__VA_ARGS__)

const char *level_to_str(LogLevel level);
void log_to_stderr(const char *buf, size_t len, const char *format, ...);
void log_to_stderr(const char *buf);

#endif  // ASSEMBLER_UTIL_LOG_H