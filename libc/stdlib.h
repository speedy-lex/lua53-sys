#ifndef _STDLIB_H
#define _STDLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define RAND_MAX (0x7fffffff)

int abs (int);
_Noreturn void abort (void);

int rand (void);
void srand (unsigned);

double strtod (const char *__restrict, char **__restrict);

void *malloc (size_t);
void *realloc (void *, size_t);
void free (void *);

char *_ltoa(long value, char *str, int radix);
char *_ultoa(unsigned long value, char *str, int radix);

char *getenv(const char *name);

#ifdef __cplusplus
}
#endif

#endif
