#ifndef _MATH_H
#define _MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <float.h>
#define HUGE_VAL  ((double)INFINITY)

double fabs(double);
double fmod(double, double);
double floor(double);
double ceil(double);
double sqrt(double);

double log(double);
double log2(double);
double log10(double);
double exp(double);
double frexp(double, int *);
double pow(double, double);

double sin(double);
double cos(double);
double tan(double);
double asin(double);
double acos(double);
double atan2(double, double);

#ifdef __cplusplus
}
#endif

#endif

