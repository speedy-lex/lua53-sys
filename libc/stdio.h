#ifndef _STDIO_H
#define _STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef void FILE;

FILE *stdout = 0;
#define stdout (stdout)

size_t fwrite(const void *__restrict, size_t, size_t, FILE *__restrict);
int fflush(FILE *);

int snprintf(char *__restrict, size_t, const char *__restrict, ...);

#ifdef __cplusplus
}
#endif

#endif