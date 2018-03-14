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
#define SIGN       8
#define UPPERCASE  16
#define ALTERNATE  32

char * print_number (char *stream, int n, int base, int field_width, const char *sigil, uint8_t attributes) {
  const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
  int n_len = 0;
  int n_tmp;
  int digit;
  char sign = 0;
  char *sp = stream;
  int pad_len = field_width;

  if (attributes & UPPERCASE) digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  if (attributes & BLANK) sign = ' ';
  if (attributes & SIGN) sign = '+';
  if (n < 0) {
    sign = '-';
    n = -n;
  }

  n_tmp = n;
  if (n == 0) n_len = 1;
  while (n_tmp > 0) {
    n_len++;
    n_tmp = (n_tmp - n_tmp % base) / base;
  }
  pad_len = field_width - n_len;

  if (sigil && (attributes & ALTERNATE)) pad_len -= strlen(sigil);
  if (sign) pad_len -= 1;

  if (!(attributes & LEFT_ALIGN)) {
    if (!(attributes & PAD_ZERO)) while (pad_len-- >= 0) *sp++ = ' ';
    if (sign) *sp++ = sign;
    if (sigil && (attributes & ALTERNATE))
      for (size_t i = 0; i < strlen (sigil); i++) *sp++ = sigil[i];
    if (attributes & PAD_ZERO) while (pad_len-- >= 0) *sp++ = '0';
  }
  else {
    if (sign) *sp++ = sign;
    if (sigil && (attributes & ALTERNATE))
      for (size_t i = 0; i < strlen (sigil); i++) *sp++ = sigil[i];
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
  int n;
  int field_width;
  int len;
  uint8_t attributes = 0;

  for (const char *fp = format; *fp; fp++) {
    if (*fp != '%') {
      *sp++ = *fp;
      continue;
    }

    field_width = -1;
  repeat_attribute:
    ++fp;
    switch (*fp) {
    case '0': attributes |= PAD_ZERO;   goto repeat_attribute;
    case '-': attributes |= LEFT_ALIGN; goto repeat_attribute;
    case ' ': attributes |= BLANK;      goto repeat_attribute;
    case '+': attributes |= SIGN;       goto repeat_attribute;
    case '#': attributes |= ALTERNATE;  goto repeat_attribute;
    }

    if (attributes & LEFT_ALIGN) attributes -= PAD_ZERO;
    if (attributes & SIGN) attributes -= BLANK;
    
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
      sp = print_number (sp, va_arg(args, int), 10, field_width, NULL, attributes); break;
    case 'X':
      attributes |= UPPERCASE;
      sp = print_number (sp, va_arg(args, int), 16, field_width, "0X", attributes); break;
    case 'x':
      sp = print_number (sp, va_arg(args, int), 16, field_width, "0x", attributes); break;
    case 'O':
      attributes |= UPPERCASE;
    case 'o':
      n = va_arg(args, int);
      sp = print_number (sp, n, 8, field_width, (n == 0 ? NULL : "0"), attributes); break;
      
    default: // unrecognised flag.
      return -1;
    }
    
  }
  *sp = '\0';
  return sp - stream;
}
