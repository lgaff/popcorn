/* Early kernel formatted output to console device. */

#include <kernel/kprintf.h>
#include <stdio.h>
#include <stdarg.h>
#include <kernel/tty.h>

char print_buffer[1024];

void kprintf (const char *fmt, ...) {
  va_list arg_ptr;
  va_start (arg_ptr, fmt);
  sprintf (print_buffer, fmt, arg_ptr);
  tty_writestring(print_buffer);
}
