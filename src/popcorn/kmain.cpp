#include <stdint.h>
#include <kernel/io.h>
#include <kernel/interrupts.h>
#include <kernel/panic.h>
#include <kernel/log.h>
#include <kernel/vga.h>


Log::Level k_loglevel = Log::Level::Debug;
VGA::TextBuffer tty0;
  


extern "C" void _kmain ()
{
  tty0.Init();
  /* Test code follows. delete me when kdev gets going. */
  /* This should still be identity mapped */
   Log::Info ("popcorn kernel 0.0.2-very-alpha");
   initialise_idt ();
   Log::Warn ("Interrupts enabled (but not configured)");
   panic();
  for(;;) {}
}
