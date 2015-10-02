#include <stdint.h>
#include <kernel/io.h>
#include <interrupts.h>
#include <kernel/kprintf.h>
#include <kernel/tty.h>
#include <stdio.h>
#include <logging.h>

log_level_t debug_level = WARNING;

void _kmain ()
{
  /* Test code follows. delete me when kdev gets going. */
  /* This should still be identity mapped */
  tty_initialise ();
  log_write (INFO, "popcorn kernel 0.0.2-very-alpha");
  initialise_idt ();
  log_write (WARNING, "Interrupts enabled (but not configured)");
  
  for(;;) {}
}
