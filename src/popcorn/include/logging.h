#ifndef _LOGGING_H
#define _LOGGING_H

typedef enum {
  INFO = 0,
  NOTICE,
  WARNING,
  ERROR,
  CRITICAL,
  OHSHI
} log_level_t;

extern log_level_t debug_level;

extern void _log_write (log_level_t, char *, int, char *, ...);

#ifndef QUIET
#define log_write(level, ...) _log_write(level, __FILE__, __LINE__, __VA_ARGS__)
#else
#define log_write(level, ...)
#endif
  

#endif /* _LOGGING_H */
