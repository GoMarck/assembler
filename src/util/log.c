#include "assembler/log.h"

#include <stdarg.h>
#include <string.h>

const char *level_to_str(LogLevel level) {
  switch (level) {
    case VERBOSE:
      return "V";
    case INFO:
      return "I";
    case WARNING:
      return "W";
    case ERROR:
      return "E";
    case FATAL:
      return "F";
    default:
      return "U";
  }
}

void log_to_stderr(char *buf, size_t len, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  vsprintf(buf, format, ap);
  fprintf(stderr, "%s", buf);
  va_end(ap);
}

void log_tail(const char *buf) { 
  fprintf(stderr, "%s", buf); 
}

const char* const_basename(const char* filepath) {
  const char* base = strrchr(filepath, '/');
#ifdef OS_WINDOWS  // Look for either path separator in Windows
  if (!base)
    base = strrchr(filepath, '\\');
#endif
  return base ? (base+1) : filepath;
}