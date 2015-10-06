#ifndef _STDIO_H
#define _STDIO_H
#include <stddef.h>

#include <sys/cdefs.h>
#include <stdarg.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
  
int putchar(int);

int sprintf (char *, const char *, ...);
int vsprintf (char *, const char *, va_list);
// int vsnprintf(char *, size_t, const char *, va_list);

#ifdef __cplusplus
}
#endif

#endif
