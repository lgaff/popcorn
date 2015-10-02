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

void log_write (log_level_t, char *, int, char *, ...);
  

#endif /* _LOGGING_H */
