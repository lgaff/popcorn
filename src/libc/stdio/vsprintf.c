/* WARNING WARNING WARNING WARNING
   This code is currently riddled with potential buffer overruns and prone to abuse. tread carefully.
   If you see somewhere that looks like it might cause nasties, it probably will, and is already known...
*/

#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#define PAD_ZERO   1
#define LEFT_ALIGN 2
#define BLANK      4

char * print_number (char *stream, int n, int base, int field_width, uint8_t attributes) {
  const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
  //char ** digits = &digits_lower;
  int n_len = 0;
  int n_tmp;
  int digit;
  char sign = 0;
  char *sp = stream;

  if (n < 0) {
    sign = '-';
    n = -n;
  }
  else if (attributes & BLANK) sign = ' ';

  n_tmp = n;
  if (n == 0) n_len = 1;
  while (n_tmp > 0) {
    n_len++;
    n_tmp = (n_tmp - n_tmp % base) / base;
  }

  if (sign) {
    if (attributes & LEFT_ALIGN || attributes & PAD_ZERO) *sp++ = sign;
    if (!(attributes & LEFT_ALIGN)) {
      while (n_len < field_width--) *sp++ = (attributes & PAD_ZERO ? '0' : ' ');
      if (!(attributes & PAD_ZERO)) *sp++ = sign;
    }
  }

  else {
    if (!(attributes & LEFT_ALIGN))
      while (n_len <= field_width--) *sp++ = (attributes & PAD_ZERO ? '0' : ' ');
  }

  n_tmp = n;
  for (int i = n_len-1; i >= 0; i--) {
    digit = n_tmp % base;
    *(sp + i) = digits[digit];
    n_tmp = (n_tmp - digit) / base;
  }

  sp += n_len;

  if (attributes & LEFT_ALIGN) {
    while (n_len <= field_width - (sign ? 1 : 0)) {
      *sp++ = ' ';
      field_width--;
    }
  }
  return sp;
}

int vsprintf(char *stream, const char *format, va_list args) {
  char *sp = stream;
  char * s;
  int field_width = -1;
  int len;
  uint8_t attributes = 0;

  for (const char *fp = format; *fp; fp++) {
    if (*fp != '%') {
      *sp++ = *fp;
      continue;
    }

  repeat_attribute:
    ++fp;
    switch (*fp) {
    case '0': attributes |= PAD_ZERO;   goto repeat_attribute;
    case '-':
      attributes |= LEFT_ALIGN;
      attributes |= !PAD_ZERO;
      goto repeat_attribute;
    case ' ': attributes |= BLANK;      goto repeat_attribute;
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
      if (!(attributes & LEFT_ALIGN))
        while (field_width-- > 0) *sp++ = ' ';
      *sp++ = (char)va_arg(args, int);
      if (attributes & LEFT_ALIGN)
        while (field_width-- > 0) *sp++ = ' ';
      break;
    case 's':
      s = va_arg(args, char *);
      len = strlen(s);
      if (!(attributes & LEFT_ALIGN))
        while (len <= field_width--) *sp++ = ' ';
      if ((attributes & BLANK) && len == 0) {
        *sp++ = ' ';
        field_width--;
      }
      while(*s) *sp++ = *s++;
      if (attributes  & LEFT_ALIGN)
        while (len <= field_width--) *sp++ = ' ';
      break;
    case 'd':
      sp = print_number (sp, va_arg(args, int), 10, field_width, attributes); break;
    default: // unrecognised flag.
      return -1;
    }      
  }
  *sp = '\0';
  return sp - stream;
}
