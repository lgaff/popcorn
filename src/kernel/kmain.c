#include <stdint.h>
#include <sys/io.h>

#include <kernel/console.h>

void _kmain ()
{
  /* Test code follows. delete me when kdev gets going. */
  /* This should still be identity mapped */
  console_initialise ();
  console_writestring ("Popcorn kernel 0.0.1-very-alpha");
  for(;;) {}
}
