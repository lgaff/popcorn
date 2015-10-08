#include <stdlib.h>
#include <kernel/log.h>

void panic () {
  Log::Fatal ("PANIC: kernel panic doesn't exist yet! DOUBLE PANIC!!");
  for (;;);
  __builtin_unreachable();
}
