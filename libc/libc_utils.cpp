// the stuff I didn't write in Rust

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>

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

        if (ca != cb) {
            return ca - cb;
        }
        if(ca == 0) { // reached terminator
            return 0;
        }
        i++;
    }
}
int strncmp(const char *a, const char *b, size_t max) {
    size_t i = 0;
    while(i < max) {
        unsigned char ca = (unsigned char)a[i];
        unsigned char cb = (unsigned char)b[i];

        if (ca != cb) {
            return ca - cb;
        }
        if(ca == 0) { // reached terminator
            return 0;
        }
        i++;
    }
	return 0;
}

char *strstr (const char *haystack, const char *needle) {
	if (!*needle) return (char *)haystack;

	for (; *haystack; ++haystack) {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && (*h == *n)) {
            ++h;
            ++n;
        }

        if (!*n) {
            return (char *)haystack; // full match found
        }
    }

	return NULL;
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

char *strerror (int) {
	return const_cast<char*>("unsupported");
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
		if(aBytes[i] != bBytes[i]) return aBytes[i] - bBytes[i];
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
FILE *_stderr = NULL;
FILE *_stdin = NULL;

int fflush(FILE *) {
	return 0;
}
int feof(FILE *) {
	return 0;
}
int ferror(FILE *) {
	return 0;
}
int getc(FILE *) {
	return EOF;
}
int fputc(int c, FILE *stream) {
	return 0;
}

size_t fread(void *__restrict, size_t, size_t, FILE *__restrict) {
	return 0;
}
size_t fwrite(const void *__restrict buf, size_t s, size_t len, FILE *__restrict f) {
	return len; // yup trust me bro
}

int fprintf(FILE *__restrict, const char *__restrict, ...) {
	return 0;
}

FILE *fopen(const char *__restrict, const char *__restrict) {
	return NULL;
}
FILE *freopen(const char *__restrict, const char *__restrict, FILE *__restrict) {
	return NULL;
}
int fclose(FILE *) {
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
static char *null_str = const_cast<char*>(""); 
char *getenv( const char *name ) {
	return null_str;
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
double difftime(time_t time_end, time_t time_beg) {
	return time_end - time_beg;
}
time_t mktime(struct tm* arg) {
	time_t time = 0;
	time += arg->tm_sec;
	time += arg->tm_min * 60;
	time += (arg->tm_hour + arg->tm_isdst) * 60 * 60;
	time += arg->tm_yday * 60 * 60 * 24;
	time += (arg->tm_year - 70) * 60 * 60 * 24 * 365;
	// idk someone do leap days and leap seconds
	return time;
}

// errno.h

static int error = 0;
int *__errno_location(void) {
	return &error;
}

// locale.h

static struct lconv locale = {
	const_cast<char*>("."),
	const_cast<char*>(","),
	const_cast<char*>("\3"),

	const_cast<char*>("USD "),
	const_cast<char*>("$"),
	const_cast<char*>("."),
	const_cast<char*>(","),
	const_cast<char*>("\3"),
	const_cast<char*>(""),
	const_cast<char*>("-"),

	2,
	2,
	1,
	0,
	1,
	0,
	1,
	1,

	1,
	0,
	1,
	0,
	1,
	1
};
struct lconv *localeconv(void) {
	return &locale;
}

// strtod

// we pretend 2nd arg is const because in this household it is
double strtod(const char *str, char**str_end) {
	// tightly packed so LLVM makes it smaller (source: it was revealed to blendi in a caffeine overdose and I made it up)
	double n = 0;
	bool negative = false;
	if(*str == '-') {
		negative = true;
		str++;
	}
	// this can kill itself
	else if(*str == '+') {
		str++;
	}
	if(!isdigit(*str)) {
		goto done; // die
	}
	while(isdigit(*str)) {
		n *= 10;
		n += *str - '0'; // peak gaming
		str++;
	}
	if(*str == '.') {
		str++;
		double m = 1;
		while(isdigit(*str)) {
			m *= 0.1; // blendi reference
			n += m * (*str - '0');
			str++; // important to blendi
		}
	}
	if(*str == 'e') {
		str++;
		int exp = 0;
		bool expNeg = false; // naming variables is my passion
		if(*str == '-') {
			expNeg = true;
			str++;
		} else if(*str == '+') {
			str++;
		}
		if(!isdigit(*str)) goto done; // invalid !!!!!!!!!
		while(isdigit(*str)) {
			exp *= 10;
			exp += *str - '0';
			str++;
		}
		for(int i = 0; i < exp; i++) n *= expNeg ? 0.1 : 10; // fine blendi, can u fuck off now
	}
done:
	if(str_end != NULL) {
		*str_end = (char *)str; // source: trust me bro
	}
	return negative ? -n : n;
}

#ifdef __cplusplus
}
#endif
