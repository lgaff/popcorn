#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

typedef struct flags {
  bool pad_zeroes;
  bool min_width;
  bool left_align;
} flags_t;

int vsprintf(char *stream, const char *format, va_list args) {
  char *sp = stream;
  char * s;
  int field_width = -1;
  int len;
  flags_t attributes = { .pad_zeroes = false,
                         .min_width = false,
                         .left_align = false
  };

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
      else if (*fp == '-')
        attributes.left_align = true;        
      else break;
      fp++;
    }
    
    // Field width Skip a sign if given.
    if (*fp == '-' || *fp == '+')
      fp++;
    if (*fp >= '1' && *fp <= '9') {
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
      if (!attributes.left_align)
        while (field_width-- > 0) *sp++ = ' ';
      *sp++ = (char)va_arg(args, int);
      if (attributes.left_align)
        while (field_width-- > 0) *sp++ = ' ';
      break;
    case 's':
      s = va_arg(args, char *);
      len = strlen(s);
      if (!attributes.left_align)
        while (len <= field_width--) *sp++ = ' ';
      while(*s) *sp++ = *s++;
      if (attributes.left_align)
        while (len <= field_width--) *sp++ = ' ';
      break;
    default: // unrecognised flag.
      return -1;
    }      
  }
  *sp = '\0';
  return sp - stream;
}
