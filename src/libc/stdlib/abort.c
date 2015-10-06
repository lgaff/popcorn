#include <stdlib.h>

__attribute__((__noreturn__))
void abort (void)
{
  for (;;);
  __builtin_unreachable();
}
