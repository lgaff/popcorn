#include <stdlib.h>
#include <kernel/tty.h>

void panic () {

  tty_writestring ("PANIC: PANIC called but PANIC does nothing yet! DOUBLE PANIC!\n");
  for (;;);
  __builtin_unreachable();
}
