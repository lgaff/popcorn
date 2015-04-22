#ifndef _KERNEL_CONSOLE_H
#define _KERNEL_CONSOLE_H

/* VGA console driver for kernel space
 * April 22 2015
 */
#include <stddef.h>

void console_initialise (void);
void console_putchar (char c);
void console_setcolor (uint8_t color);
void console_write (const char *str, size_t size);
void console_writestring (const char *str);

#endif
