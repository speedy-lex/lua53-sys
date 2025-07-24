#ifndef _STDIO_H
#define _STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define EOF (-1)

#define BUFSIZ 1024

typedef void FILE;


extern FILE *_stdin;
#define stdin (_stdin)
extern FILE *_stdout;
#define stdout (_stdout)
extern FILE *_stderr;
#define stderr (_stderr)

FILE *fopen(const char *__restrict, const char *__restrict);
FILE *freopen(const char *__restrict, const char *__restrict, FILE *__restrict);
int fclose(FILE *);

size_t fread(void *__restrict, size_t, size_t, FILE *__restrict);
size_t fwrite(const void *__restrict, size_t, size_t, FILE *__restrict);
int fflush(FILE *);
int feof(FILE *);
int ferror(FILE *);
int getc(FILE *);

int fprintf(FILE *__restrict, const char *__restrict, ...);
int snprintf(char *__restrict, size_t, const char *__restrict, ...);

#ifdef __cplusplus
}
#endif

#endif
