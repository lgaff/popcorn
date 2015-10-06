#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>

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
    memmove (tty_buffer, (const void *)(tty_buffer + VGA_WIDTH * 2), VGA_WIDTH * 2 * (VGA_HEIGHT - 1));
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
  default: tty_putat (c, tty_color, tty_column, tty_row); tty_column++; break;
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


