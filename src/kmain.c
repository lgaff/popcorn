#include <stdint.h>

void _kmain ()
{
  /* This should still be identity mapped */
  uint16_t *buf = (uint16_t *)0xB8000;
  uint8_t attribute = 15 | 0 << 4;
  uint16_t test_char = 'y' | (uint16_t)attribute << 8;
  *buf = test_char;
  for(;;) {}
}
