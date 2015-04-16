#include <stdint.h>
#include <sys/io.h>
void _kmain ()
{
  /* Test code follows. delete me when kdev gets going. */
  /* This should still be identity mapped */
  uint16_t *buf = (uint16_t *)0xB8000;
  uint16_t console_port = 0x3F8;
  uint8_t attribute = 15 | 0 << 4;
  uint16_t test_char = 'y' | (uint16_t)attribute << 8;
  *buf = test_char;
  /* Console output: */
  outb (console_port, 'O');
  outb (console_port, 'K');
  for(;;) {}
}
