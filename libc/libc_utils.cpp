// the stuff I didn't write in Rust

#include <stddef.h>
#include <stdio.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// string.h

char *strchr(const char *s, int b) {
	size_t i = 0;
	while(s[i]) {
		if(s[i] == b) return (char *)(s + i);
		i++;
	}
	return NULL;
}

size_t strspn(const char *dest, const char *src) {
	size_t len = 0;
	while(dest[len]) {
		if(strchr(src, dest[len]) == NULL) break;
		len++;
	}
	return len;
}

size_t strlen(const char *s) {
	size_t len = 0;
	while(s[len]) len++;
	return len;
}

int strcmp(const char *a, const char *b) {
    size_t i = 0;
    while(1) {
        unsigned char ca = (unsigned char)a[i];
        unsigned char cb = (unsigned char)b[i];

        if(ca < cb) {
            return -1;
        }
        if(ca > cb) {
            return -1;
        }
        if(ca == 0 && cb == 0) { // reached terminator
            return 0;
        }
        i++;
    }
}

char *strpbrk (const char *dest, const char *breakset) {
	for(size_t i = 0; dest[i]; i++) {
		if(strchr(breakset, dest[i]) != NULL) {
			return (char *)(dest + i);
		}
	}
	return NULL;
}

char *strcpy(char *dest, const char *src) {
    if(dest == NULL) return dest;
    size_t i = 0;
    while(src[i]) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = 0;
    return dest;
}

int strcoll (const char *a, const char *b) {
	// fuck locales
	return strcmp(a, b);
}

void *memset(void *buf, int byte, size_t len) {
    if(buf == NULL) return NULL;
    unsigned char *bytes = (unsigned char *)buf;
    for(size_t i = 0; i < len; i++) bytes[i] = byte;
	return buf;
}

void *memcpy(void *dest, const void *src, size_t len) {
    if(dest == NULL) return NULL;
    if(src == NULL) return NULL;
    if(len == 0) return dest;

    char *destBytes = (char *)dest;
    const char *srcBytes = (const char *)src;
    for(size_t i = 0; i < len; i++) {
        destBytes[i] = srcBytes[i];
    }
	return dest;
}

int memcmp (const void *a, const void *b, size_t len) {
	const unsigned char *aBytes = (const unsigned char *)a;
	const unsigned char *bBytes = (const unsigned char *)b;

	for(size_t i = 0; i < len; i++) {
		if(aBytes[i] < bBytes[i]) return -1;
		if(aBytes[i] > bBytes[i]) return 1;
	}
	return 0;
}

void *memchr (const void *buf, int ch, size_t len) {
	const unsigned char *bytes = (const unsigned char *)buf;
	for(size_t i = 0; i < len; i++) {
		if(bytes[i] == ch) return (void *)(bytes + i);
	}
	return NULL;
}

// stdio.h (-snprintf)
FILE *_stdout = NULL;

size_t fwrite(const void *__restrict buf, size_t s, size_t len, FILE *__restrict f) {
	return len; // yup trust me bro
}

int fflush(FILE *f) {
	return 0;
}

// stdlib.h (-strtod)

int abs (int x) {
	if(x < 0) return -x;
	return x;
}
_Noreturn void abort (void) {
	__builtin_trap();
}

// taken from https://git.musl-libc.org/cgit/musl/tree/src/prng/rand.c
unsigned long long _rand_seed = 694201337;

int rand (void) {
	_rand_seed = 6364136223846793005ULL*_rand_seed + 1;
	return _rand_seed >> 33;
}

void srand (unsigned seed) {
	_rand_seed = seed-1;
}

// time.h (-time)

clock_t clock (void) {
	return 0;
}

#ifdef __cplusplus
}
#endif
