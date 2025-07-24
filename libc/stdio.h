#ifndef _STDIO_H
#define _STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#define BUFSIZ 1024

#define EOF (-1)

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef void FILE;

FILE *stdin = 0;
FILE *stdout = 0;
FILE *stderr = 0;
#define stdin (stdin)
#define stdout (stdout)
#define stderr (stderr)

size_t fread(void *__restrict, size_t, size_t, FILE *__restrict);
size_t fwrite(const void *__restrict, size_t, size_t, FILE *__restrict);
int fflush(FILE *);
int ferror(FILE *);
int feof(FILE *);

int fprintf(FILE *__restrict, const char *__restrict, ...);
int snprintf(char *__restrict, size_t, const char *__restrict, ...);
char *fgets(char *__restrict, int, FILE *__restrict);
int getc(FILE *);

FILE *fopen(const char *__restrict, const char *__restrict);
FILE *freopen(const char *__restrict, const char *__restrict, FILE *__restrict);
int fclose(FILE *);

#ifdef __cplusplus
}
#endif

#endif