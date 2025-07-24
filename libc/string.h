#ifndef _STRING_H
#define _STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

char *strchr (const char *, int);
size_t strspn (const char *, const char *);
size_t strlen (const char *);
int strcmp (const char *, const char *);
char *strpbrk (const char *, const char *);
char *strcpy(char *__restrict, const char *__restrict);
int strcoll (const char *, const char *);

void *memset(void *buf, int byte, size_t len);
void *memcpy(void *, const void *, size_t);
int memcmp (const void *, const void *, size_t);
void *memchr (const void *, int, size_t);

#ifdef __cplusplus
}
#endif

#endif
