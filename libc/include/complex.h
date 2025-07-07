/* *****************************************************************************
   MIT License

   Copyright (c) 2023-2024 I.F.F. dos SANTOS

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
#ifndef COMPLEX_H
#define COMPLEX_H 1
#include <complex.h>
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
/* ISO/IEC 9899:1999
   7.3 Complex arithmetic <complex.h> */

#define complex _Complex
#define imaginary _Imaginary



#if defined(__GNUC__)
   /* GNU C compatible source code. */
   #define _Complex_I (__extension__ 1.0iF)
   #define I _Complex_I
#else
   #define _Complex_I (((union {\
      float _Complex m_complex; float ma_real[2];\
   }){ .ma_real = {0.0e+00f, 1.0e+00f}}).m_complex )
   #define I _Complex_I
#endif



#pragma STDC CX_LIMITED_RANGE off /* ??? */



#define __CMPLX_DECLS0(name) \
   double      name   (double      _Complex); \
   float       name##f(float       _Complex); \
   long double name##l(long double _Complex);
#define __CMPLX_DECLS1(name) \
   double      _Complex name   (double      _Complex); \
   float       _Complex name##f(float       _Complex); \
   long double _Complex name##l(long double _Complex);
#define __CMPLX_DECLS2(name) \
   double      _Complex name   (double      _Complex, double      _Complex); \
   float       _Complex name##f(float       _Complex, float       _Complex); \
   long double _Complex name##l(long double _Complex, long double _Complex);

__CMPLX_DECLS1(cacos)
__CMPLX_DECLS1(casin)
__CMPLX_DECLS1(catan)
__CMPLX_DECLS1(ccos)
__CMPLX_DECLS1(csin)
__CMPLX_DECLS1(ctan)
__CMPLX_DECLS1(cacosh)
__CMPLX_DECLS1(casinh)
__CMPLX_DECLS1(catanh)
__CMPLX_DECLS1(ccosh)
__CMPLX_DECLS1(csinh)
__CMPLX_DECLS1(ctanh)
__CMPLX_DECLS1(cexp)
__CMPLX_DECLS1(clog)
__CMPLX_DECLS2(cpow)
__CMPLX_DECLS1(csqrt)
__CMPLX_DECLS1(conj)
__CMPLX_DECLS1(cproj)

__CMPLX_DECLS0(creal)
__CMPLX_DECLS0(cabs)
__CMPLX_DECLS0(carg)
__CMPLX_DECLS0(cimag)

#undef __CMPLX_DECLS0
#undef __CMPLX_DECLS1
#undef __CMPLX_DECLS2

#endif /* ISO C99 */





#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
/* ISO/IEC 9899:2011
   7.3 Complex arithmetic <complex.h> */

#if defined(_Imaginary_I)
   #define __CMPLX(x, y, type) ((type)(x) + _Imaginary_I * (type)(y))
#elif defined(__GNUC__)
   #define __CMPLX(x, y, type) __builtin_complex((type)(x), (type)(y))
#else
   #define __CMPLX(x, y, type) (((union {\
      type _Complex m_complex; type ma_real[2];\
   }){ .ma_real = {(type)(x), (type)(y) }}).m_complex)
#endif /* _Imaginary_I */

#define CMPLX(x, y)  __CMPLX(x, y, double)
#define CMPLXF(x, y) __CMPLX(x, y, float)
#define CMPLXL(x, y) __CMPLX(x, y, long double)

#endif /* ISO C11 */

#ifdef __cplusplus
}
#endif

#endif /* COMPLEX_H */
