/* *****************************************************************************
   MIT License

   Copyright (c) 2023-2024 I.F.F. dos Santos

   Permission is hereby granted, free of charge, to any person obtaining a copy 
   of this software and associated documentation files (the “Software”), to 
   deal in the Software without restriction, including without limitation the 
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
   sell copies of the Software, and to permit persons to whom the Software is 
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in 
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
   IN THE SOFTWARE.
***************************************************************************** */
#ifndef MATH_H
#define _MATH_H 1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __STDC__
/* ANSI C, C89 (ANSI X3.159-1989) compatible source code. */

/* One parameter */
double fabs(double x);
double exp(double x);
double log(double x);
double log10(double x);
double sqrt(double x);
double acos(double x);
double asin(double x);
double atan(double x);
double cos(double x);
double sin(double x);
double tan(double x);
double cosh(double x);
double sinh(double x);
double tanh(double x);
double ceil(double x);
double floor(double x);

/* Two parameters */
double fmod(double x, double y);
double pow(double x, double y);
double atan2(double y, double x);
double frexp(double value, int *exp);
double ldexp(double x, int exp);
double modf(double value, double *iptr);
#endif /* __STDC__ */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
/* C99 (ISO/IEC 9899:1999) compatible source code. */

float       acosf(float x);
long double acosl(long double x);

float       asinf(float x);
long double asinl(long double x);

float       atanf(float x);
long double atanl(long double x);

float       atan2f(float y, float x);
long double atan2l(long double y, long double x);

float       cosf(float x);
long double cosl(long double x);

float       sinf(float x);
long double sinl(long double x);

float       tanf(float x);
long double tanl(long double x);

double      acosh(double x);
float       acoshf(float x);
long double acoshl(long double x);

double      asinh(double x);
float       asinhf(float x);
long double asinhl(long double x);

double      atanh(double x);
float       atanhf(float x);
long double atanhl(long double x);

float       coshf(float x);
long double coshl(long double x);

float       sinhf(float x);
long double sinhl(long double x);

float       tanhf(float x);
long double tanhl(long double x);

float       expf(float x);
long double expl(long double x);

double      exp2(double x);
float       exp2f(float x);
long double exp2l(long double x);

double      expm1(double x);
float       expm1f(float x);
long double expm1l(long double x);

float       frexpf(float value, int *exp);
long double frexpl(long double value, int *exp);

int ilogb(double x);
int ilogbf(float x);
int ilogbl(long double x);

float       ldexpf(float x, int exp);
long double ldexpl(long double x, int exp);

float       logf(float x);
long double logl(long double x);

float       log10f(float x);
long double log10l(long double x);

double      log1p(double x);
float       log1pf(float x);
long double log1pl(long double x);

double      log2(double x);
float       log2f(float x);
long double log2l(long double x);

double      logb(double x);
float       logbf(float x);
long double logbl(long double x);

float       modff(float value, float *iptr);
long double modfl(long double value, long double *iptr);

double      scalbn(double x, int n);
float       scalbnf(float x, int n);
long double scalbnl(long double x, int n);

double      scalbln(double x, long int n);
float       scalblnf(float x, long int n);
long double scalblnl(long double x, long int n);

double      cbrt(double x);
float       cbrtf(float x);
long double cbrtl(long double x);

float       fabsf(float x);
long double fabsl(long double x);

double      hypot(double x, double y);
float       hypotf(float x, float y);
long double hypotl(long double x, long double y);

float       powf(float x, float y);
long double powl(long double x, long double y);

float       sqrtf(float x);
long double sqrtl(long double x);

double      erf(double x);
float       erff(float x);
long double erfl(long double x);

double      erfc(double x);
float       erfcf(float x);
long double erfcl(long double x);

double      lgamma(double x);
float       lgammaf(float x);
long double lgammal(long double x);

double      tgamma(double x);
float       tgammaf(float x);
long double tgammal(long double x);

float       ceilf(float x);
long double ceill(long double x);

float       floorf(float x);
long double floorl(long double x);

double      nearbyint(double x);
float       nearbyintf(float x);
long double nearbyintl(long double x);

double      rint(double x);
float       rintf(float x);
long double rintl(long double x);

long int lrint(double x);
long int lrintf(float x);
long int lrintl(long double x);

long long int llrint(double x);
long long int llrintf(float x);
long long int llrintl(long double x);

double      round(double x);
float       roundf(float x);
long double roundl(long double x);

long int lround(double x);
long int lroundf(float x);
long int lroundl(long double x);

long long int llround(double x);
long long int llroundf(float x);
long long int llroundl(long double x);

double      trunc(double x);
float       truncf(float x);
long double truncl(long double x);

float       fmodf(float x, float y);
long double fmodl(long double x, long double y);

double      fmod(double x, double y);
float       fmodf(float x, float y);
long double fmodl(long double x, long double y);

double      remquo(double x, double y, int *quo);
float       remquof(float x, float y, int *quo);
long double remquol(long double x, long double y,int *quo);

double      copysign(double x, double y);
float       copysignf(float x, float y);
long double copysignl(long double x, long double y);

double      nan(const char *tagp);
float       nanf(const char *tagp);
long double nanl(const char *tagp);

double      nextafter(double x, double y);
float       nextafterf(float x, float y);
long double nextafterl(long double x, long double y);

double      nexttoward(double x, long double y);
float       nexttowardf(float x, long double y);
long double nexttowardl(long double x, long double y);

double      fdim(double x, double y);
float       fdimf(float x, float y);
long double fdiml(long double x, long double y);

double      fmax(double x, double y);
float       fmaxf(float x, float y);
long double fmaxl(long double x, long double y);

double      fmin(double x, double y);
float       fminf(float x, float y);
long double fminl(long double x, long double y);

double      fma(double x, double y, double z);
float       fmaf(float x, float y, float z);
long double fmal(long double x, long double y, long double z);

#define HUGE_VAL /* implementation-defined */
#define HUGE_VALF /* implementation-defined */
#define HUGE_VALL /* implementation-defined */

#define INFINITY /* IEEE-754 Infinity */
#define NAN /* IEEE-754 Not A Number */

#define FP_INFINITE /* implementation-defined */
#define FP_NAN /* implementation-defined */
#define FP_NORMAL /* implementation-defined */
#define FP_SUBNORMAL /* implementation-defined */
#define FP_ZERO /* implementation-defined */

#define FP_FAST_FMA 1
#define FP_FAST_FMAF 1
#define FP_FAST_FMAL 1

#define FP_ILOGB0 /*implementation-defined*/
#define FP_ILOGBNAN /*implementation-defined*/

#define MATH_ERRNO 1
#define MATH_ERREXCEPT 2
#define math_errhandling (MATH_ERRNO | MATH_ERREXCEPT)

#define FP_NORMAL /*implementation defined*/
#define FP_SUBNORMAL /*implementation defined*/
#define FP_ZERO /*implementation defined*/
#define FP_INFINITE /*implementation defined*/
#define FP_NAN /*implementation defined*/

#define fpclassify(x) /* implementation defined */
#define isfinite(x) /* implementation defined */
#define isinf(x) /* implementation defined */
#define isnan(x) /* implementation defined */
#define isnormal(x) /* implementation defined */
#define signbit(x) /* implementation defined */

#define isgreater(x, y) /* implementation defined */
#define isgreaterequal(x, y) /* implementation defined */
#define isless(x, y) /* implementation defined */
#define islessequal(x, y) /* implementation defined */
#define islessgreater(x, y) /* implementation defined */
#define isunordered(x, y) /* implementation defined */

#endif /* ISO C99 */

#ifdef __cplusplus
}
#endif

#endif /* MATH_H */
