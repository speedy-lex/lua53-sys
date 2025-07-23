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

#ifdef __cplusplus
}
#endif

#endif