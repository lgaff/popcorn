#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

typedef struct flags {
  bool pad_zeroes;
  bool min_width;
} flags_t;

int pad_field(char *stream, const char padchar, int size) {
  char *sp = stream;
  for (int i = 0; i < size; i++) {
    *sp++ = padchar;
  }
  return sp - stream;
}

int vsprintf(char *stream, const char *format, va_list args) {
  char *sp = stream;
  char * s;
  flags_t attributes = { .pad_zeroes = false, .min_width = false };
  int field_width = 0;
  for (const char *fp = format; *fp; fp++) {
    if (*fp != '%') {
      *sp++ = *fp;
      continue;
    }
    ++fp;

    // Check attributes. Yes, an arbitrary number of flags.. There is probably a better way than this.
    while (*fp) {
      if (*fp == '0') {
        attributes.pad_zeroes = true;
      }
      else break;
      fp++;
    }

    // Field width
    if (*fp >= '1' && *fp <= '9') {
      attributes.min_width = true;
      field_width += *fp++ - '0';
      while (*fp >= '0' && *fp <= '9' && *fp) {
        field_width *= 10;
        field_width += *fp - '0';
        fp++;
      }
    }
    
    switch (*fp) {
    case '%': *sp++ = '%'; break;
    case 'c':
      if (attributes.min_width) {
        sp += pad_field(sp, ' ', field_width - 1);
      }
        *sp++ = (char)va_arg(args, int); break;
    case 's':
      s = (char *)va_arg(args, char *);
      if (attributes.min_width) {
        if (field_width > (int)strlen (s))
          sp += pad_field (sp, ' ', field_width - strlen(s));
      }
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
