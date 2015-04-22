#ifndef _KERNEL_VGA_H
#define _KERNEL_VGA_H

/* vga.h - kernel-space VGA primitives to implement a console driver
 * April 22 2015
 */
#include <stdint.h>

enum VGA_COLOR
{
  COLOR_BLACK      = 0,
  COLOR_BLUE       = 1,
  COLOR_GREEN      = 2,
  COLOR_CYAN       = 3,
  COLOR_RED        = 4,
  COLOR_MAGENTA    = 5,
  COLOR_BROWN      = 6,
  COLOR_LT_GREY    = 7,
  COLOR_DK_GREY    = 8,
  COLOR_LT_BLUE    = 9,
  COLOR_LT_GREEN   = 10,
  COLOR_LT_CYAN    = 11,
  COLOR_LT_RED     = 12,
  COLOR_LT_MAGENTA = 13,
  COLOR_LT_BROWN   = 14,
  COLOR_WHITE      = 15
};

#define VGA_ATTRIBUTE(b, f) (uint8_t) (b << 4 | f & 0x0F)
#define VGA_CELL(c, a) (uint16_t)c | (uint16_t)a << 8

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static uint16_t* const VGA_BUFFER = (uint16_t *)0xB8000;

#endif
