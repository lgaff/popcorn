#include <stdio.h>
#include <stdarg.h>

//__attribute__((__format__(__printf__, 2, 3)))
int sprintf (char *stream, const char *fmt, ...) {

  int ret = 0;
  va_list arg_ptr;
  va_start (arg_ptr, fmt);
  ret = vsprintf (stream, fmt, arg_ptr);
  return ret;
}
