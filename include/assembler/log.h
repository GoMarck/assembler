#pragma once

#ifndef ASSEMBLER_LOG_H
#define ASSEMBLER_LOG_H

#include <stdio.h>
#include <stdlib.h>

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

#define LOG_LEVEL VERBOSE

#define LOG_TO_STDERR(level, ...)                                           \
  do {                                                                      \
    if (level < LOG_LEVEL) {                                                \
      break;                                                                \
    }                                                                       \
    log_to_stderr(message_prefix, sizeof(message_prefix), "%s %s:%d] ",     \
                  level_to_str(level), const_basename(__FILE__), __LINE__); \
    log_to_stderr(message_text, sizeof(message_text), __VA_ARGS__);         \
    log_tail("\n");                                                         \
    if (level == FATAL) {                                                   \
      abort();                                                              \
    }                                                                       \
  } while (0)

#define STDERR_LOG_VERBOSE(...) LOG_TO_STDERR(VERBOSE, __VA_ARGS__)
#define STDERR_LOG_INFO(...) LOG_TO_STDERR(INFO, __VA_ARGS__)
#define STDERR_LOG_WARNING(...) LOG_TO_STDERR(WARNING, __VA_ARGS__)
#define STDERR_LOG_ERROR(...) LOG_TO_STDERR(ERROR, __VA_ARGS__)
#define STDERR_LOG_FATAL(...) LOG_TO_STDERR(FATAL, __VA_ARGS__)

// Macros for logging some message to stderr.
//
//    * LOG(VERBOSE, ...):        Log verbose message to stderr.
//    * LOG(INFO, ...):           Log info message to stderr.
//    * LOG(WARNING, ...):        Log warning message to stderr.
//    * LOG(ERROR, ...):          Log error message to stderr.
//    * LOG(FATAL, ...):          Log fatal message to stderr and abort.
//
// There are 5 *Log Level* defined, from low to high show as follow:
//
//    VERBOSE: log message may very long and verbose, default off.
//    INFO:    log message is the common information, default level.
//    WARNING: log message contains some useful warning information.
//    ERROR:   log message contains error information.
//    FATAL:   the program go into some error status, log the message
//             and exit the program abnormally.
//
// Notes:
//
//    If you don't want to print some logs for some purpose, marco
//    *LOG_LEVEL* would be helpful, you can set it to the higher
//    level (e.g ERROR) to mask out low-level log output.
//
// Examples:
//
//    LOG(INFO, "Hello World!")
//    LOG(INFO, "Hello %s, we have meets %d times.", name, count)

#define LOG(level, ...) STDERR_LOG_##level(__VA_ARGS__)

const char *level_to_str(LogLevel level);
void log_to_stderr(char *buf, size_t len, const char *format, ...);
void log_tail(const char *buf);
const char* const_basename(const char* filepath);

#endif  // ASSEMBLER_LOG_H