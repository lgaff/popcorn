#include <stdio.h>

#ifdef __is_popcorn_kernel
#include <kernel/tty.h>
#endif

int putchar(int c) {
  #ifdef __is_popcorn_kernel
  tty_putchar (c);
  #else
  // TODO: Implement userland
  #endif
  return c;
}
