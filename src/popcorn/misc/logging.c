#include <logging.h>
#include <stdarg.h>
#include <stdio.h>
#include <kernel/kprintf.h>

static char * log_level_str[] = {
  "INFO",
  "NOTICE",
  "WARNING",
  "ERROR",
  "CRITICAL",
  "OHSHI-"
};

char log_buffer [1024];
void _log_write (log_level_t loglevel, char * title, int line_no, char *fmt, ...) {
  if (loglevel >= debug_level) {
    va_list arg_ptr;
    va_start (arg_ptr, fmt);
    sprintf (log_buffer, fmt, arg_ptr);
    va_end (arg_ptr);
    
    char * type;
    if (loglevel > OHSHI) {
      type = "INV_LVL";
    } else type = log_level_str[loglevel];

    kprintf ("[%8s] %s:%-d >> %s\n", type, title, line_no, log_buffer);
  }
  /* Otherwise ignore */
}
