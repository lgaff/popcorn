#include <stdint.h>
#include <kernel/io.h>
#include <interrupts.h>
#include <kernel/tty.h>

extern "C" void _kmain ()
{
  /* Test code follows. delete me when kdev gets going. */
  /* This should still be identity mapped */
  tty_initialise ();
  tty_writestring ("Popcorn kernel 0.0.1-very-alpha\n");
  initialise_idt ();
  tty_writestring ("Interrupts enabled (but not configured)\n");
  
  for(;;) {}
}
