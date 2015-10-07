#include <stdint.h>
#include <kernel/io.h>
#include <interrupts.h>
#include <kernel/kprintf.h>
#include <kernel/tty.h>
#include <kernel/panic.h>
#include <stdio.h>
#include <logging.h>


Log::Level k_loglevel = Log::Level::Debug;

extern "C" void _kmain ()
{

  /* Test code follows. delete me when kdev gets going. */
  /* This should still be identity mapped */
  tty_initialise ();
  Log::Info ("popcorn kernel 0.0.2-very-alpha");
  initialise_idt ();
  Log::Warn ("Interrupts enabled (but not configured)");
  const unsigned long *foo = (unsigned long *)0xDEADC0DE;
  kprintf("%s", foo);
  panic();
  for(;;) {}
}
