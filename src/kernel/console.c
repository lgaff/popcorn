#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>

// Internal variables
size_t console_row;
size_t console_column;
uint8_t console_color;
static uint16_t *console_buffer;

void console_initialise (void)
{
  console_row = 0;
  console_column = 0;
  console_color = VGA_ATTRIBUTE(COLOR_BLACK, COLOR_WHITE);
  console_buffer = VGA_BUFFER;

  /* Now clear the screen */
  for (size_t y = 0; y < VGA_HEIGHT; y++)
  {
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
      const size_t index = y * VGA_WIDTH + x;
      console_buffer[index] = VGA_CELL(' ', console_color);
    }
  }
}

void console_scroll()
{
  console_column = 0;
  console_row++;
  if (console_row == VGA_HEIGHT)
  {
    // scroll that shit cracka
    memmove (console_buffer, (const void *)console_buffer + VGA_WIDTH, VGA_WIDTH * (VGA_HEIGHT - 1));
    console_row = VGA_HEIGHT - 1;
  }
}

void console_setcolor (uint8_t color)
{
  console_color = color;
}

void console_putat (char c, uint8_t color, size_t x, size_t y)
{
  const size_t index = y * VGA_WIDTH + x;
  console_buffer [index] = VGA_CELL (c, color);
}

void console_putchar (char c) {
  console_putat (c, console_color, console_column, console_row);
  if (++console_column == VGA_WIDTH)
    console_scroll ();
}

void console_write (const char *str, size_t size)
{
  for (size_t i = 0;i < size; i++)
  {
    console_putchar(str[i]);
  }
}

void console_writestring (const char *str)
{
  console_write (str, strlen(str));
}


