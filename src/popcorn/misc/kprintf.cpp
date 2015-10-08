/* Early kernel formatted output to console device. */

#include <stdio.h>
#include <stdarg.h>
#include <kernel/kprintf.h>
#include <kernel/vga.h>

char print_buffer[1024];

void kprintf (const char *fmt, ...) {
  va_list arg_ptr;
  va_start (arg_ptr, fmt);
  vsprintf (print_buffer, fmt, arg_ptr);
  va_end (arg_ptr);
  tty0.Write (print_buffer);
}
