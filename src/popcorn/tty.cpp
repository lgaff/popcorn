#include <stddef.h>
#include <stdint.h>
#include <string.h>

//#include <kernel/vga.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_BUFFER = (uint16_t *)0xB8000;

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


// Internal variables
size_t tty_row;
size_t tty_column;
uint8_t tty_color;
static uint16_t *tty_buffer;

void tty_putat (char c, uint8_t color, size_t x, size_t y)
{
  const size_t index = y * VGA_WIDTH + x;
  tty_buffer [index] = VGA_CELL (c, color);
}

void tty_initialise (void)
{
  tty_row = 0;
  tty_column = 0;
  tty_color = VGA_ATTRIBUTE(COLOR_BLACK, COLOR_WHITE);
  tty_buffer = VGA_BUFFER;

  /* Now clear the screen */
  for (size_t y = 0; y < VGA_HEIGHT; y++)
  {
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
      const size_t index = y * VGA_WIDTH + x;
      tty_buffer[index] = VGA_CELL(' ', tty_color);
    }
  }
}

void tty_scroll()
{
  tty_column = 0;
  tty_row++;
  if (tty_row >= VGA_HEIGHT)
  {
    // scroll that shit cracka
    memmove (tty_buffer, (const void *)(tty_buffer + VGA_WIDTH), VGA_WIDTH * 2 * (VGA_HEIGHT - 1));
    tty_row = VGA_HEIGHT - 1;
    for (size_t x = 0; x < VGA_WIDTH; x++)
      tty_putat (' ', tty_color, x, tty_row);
  }
}

void tty_setcolor (uint8_t color)
{
  tty_color = color;
}

void tty_putchar (char c) {
  switch (c) {
  case '\n': tty_scroll(); break;
  default:
    tty_putat (c, tty_color, tty_column++, tty_row);
    break;
  }
  if (tty_column >= VGA_WIDTH)
    tty_scroll ();
}

void tty_write (const char *str, size_t size)
{
  for (size_t i = 0;i < size; i++)
  {
    tty_putchar(str[i]);
  }
}

void tty_writestring (const char *str)
{
  tty_write (str, strlen(str));
}


