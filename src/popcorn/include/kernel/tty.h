#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

/* VGA console driver for kernel space
 * April 22 2015
 */
#include <stddef.h>

void tty_initialise (void);
void tty_putchar (char c);
//void tty_setcolor (uint8_t color);
void tty_write (const char *str, size_t size);
void tty_writestring (const char *str);

#endif
