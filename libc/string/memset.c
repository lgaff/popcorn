#include <string.h>

void *memset (void *d, int val, size_t n)
{
  unsigned char *dst = (unsigned char *) d;

  for (size_t i = 0; i < n; i++)
    dst[i] = val;
  return d;
}
