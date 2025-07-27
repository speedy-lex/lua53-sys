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

struct tm {
   int tm_sec;         /* seconds,  range 0 to 59          */
   int tm_min;         /* minutes, range 0 to 59           */
   int tm_hour;        /* hours, range 0 to 23             */
   int tm_mday;        /* day of the month, range 1 to 31  */
   int tm_mon;         /* month, range 0 to 11             */
   int tm_year;        /* The number of years since 1900   */
   int tm_wday;        /* day of the week, range 0 to 6    */
   int tm_yday;        /* day in the year, range 0 to 365  */
   int tm_isdst;       /* daylight saving time             */	
};

struct tm *localtime(const time_t *timer);
struct tm *gmtime   (const time_t* timer);
time_t mktime       (struct tm* arg);
double difftime     (time_t time_end, time_t time_beg);
size_t strftime     (char* str, size_t count,
                     const char*, const struct tm*);

#ifdef __cplusplus
}
#endif

#endif
