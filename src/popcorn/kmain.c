#include <stdint.h>
#include <kernel/io.h>
#include <interrupts.h>
#include <kernel/kprintf.h>
#include <kernel/tty.h>

void _kmain ()
{
  /* Test code follows. delete me when kdev gets going. */
  /* This should still be identity mapped */
  tty_initialise ();
  kprintf (buf, "Popcorn kernel 0.0.1-very-alpha\n");
  tty_writestring (buf);
  initialise_idt ();
  kprintf ("Interrupts enabled (but not configured)\n");
  
  for(;;) {}
}
