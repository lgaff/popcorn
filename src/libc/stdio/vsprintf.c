#include <stdio.h>

typedef struct flags {
  char alternate;
  char pad_zeroes;
  char left_align;
  char add_blank;
  char add_sign;
  char precision_set;
} flags_t;

int pad_field(char *stream, const char padchar, int size) {
  char *sp = stream;
  for (int i = 0; i < size; i++) {
    *sp++ = padchar;
  }
  return *sp - *stream;
}

int vsprintf(char *stream, const char *format, va_list args) {
  char *sp = stream;
  char * s;
  for (const char *fp = format; *fp; fp++) {
    if (*fp != '%') {
      *sp++ = *fp;
      continue;
    }
    ++fp;
    switch (*fp) {
    case '%': *sp++ = '%'; break;
    case 'c': *sp++ = (char)va_arg(args, int); break;
    case 's':
      s = (char *)va_arg(args, char *);
      while(*s) {
        *sp++ = *s++;
      }
      break;
    default: // unrecognised flag.
      return -1;
    }      
  }
  *sp = '\0';
  return sp - stream;
}
