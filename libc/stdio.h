#ifndef _STDIO_H
#define _STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef void FILE;

FILE *stdin = 0;
FILE *stdout = 0;
FILE *stderr = 0;
#define stdin (stdin)
#define stdout (stdout)
#define stderr (stderr)

size_t fwrite(const void *__restrict, size_t, size_t, FILE *__restrict);
int fflush(FILE *);
int fprintf(FILE *__restrict, const char *__restrict, ...);
char *fgets(char *__restrict, int, FILE *__restrict);

#ifdef __cplusplus
}
#endif

#endif