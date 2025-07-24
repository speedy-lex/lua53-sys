#ifndef _TIME_H
#define _TIME_H

#ifdef __cplusplus
extern "C" {
#endif

typedef long long time_t;
typedef int clock_t;

#define CLOCKS_PER_SEC 1

time_t time (time_t *);
clock_t clock (void);

#ifdef __cplusplus
}
#endif

#endif
