#ifndef _KPRINTF_H
#define _KPRINTF_H
#include <kernel/vga.h>

extern VGA::TextBuffer tty0;

void kprintf (const char *, ...);


#endif /* _KPRINTF_H */
