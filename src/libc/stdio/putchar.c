#include <stdio.h>

#ifdef __popcorn_kernel
#include <kernel/tty.h>
#endif

int putchar(int c) {
  #ifdef __popcorn_kernel
  tty_putchar (c);
  #else
  // TODO: Implement userland
  #endif
  return c;
}
