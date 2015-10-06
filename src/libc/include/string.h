#ifndef _STRING_H
#define _STRING_H

#include <sys/cdefs.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


void *memmove (void *, const void *, size_t);
void *memcpy (void *, const void *, size_t);
void *memset (void *, int, size_t);
int memcmp (const void *, const void *, size_t);
size_t strlen (const char *str);

#ifdef __cplusplus
}
#endif


#endif
