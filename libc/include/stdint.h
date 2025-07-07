/* *****************************************************************************
   MIT License

   Copyright (c) 2024 I.F.F. dos SANTOS

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
#ifndef _STDINT_H_
#define _STDINT_H_
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 199901L
/* *****************************************************************************
   Standard header <stdint.h>.
   Last modified: Octubre 22, 2024.
***************************************************************************** */

#if defined(__LP32__)
   typedef signed char int8_t;
   typedef unsigned char uint8_t;
   #define INT8_C(N) N
   #define UINT8_C(N) N

   typedef signed short int int16_t;
   typedef unsigned short int uint16_t;
   #define INT16_C(N) N
   #define UINT16_C(N) N

   typedef long int int32_t;
   typedef unsigned long int uint32_t;
   #define INT32_C(N) N ## L
   #define UINT32_C(N) N ## UL

   typedef uint32_t uintptr_t;
   #define UINTPTR_MAX UINT32_C(0xffffffff)
#elif defined(__ILP32__)
   typedef signed char int8_t;
   typedef unsigned char uint8_t;
   #define INT8_C(N) N
   #define UINT8_C(N) N

   typedef signed short int int16_t;
   typedef unsigned short int uint16_t;
   #define INT16_C(N) N
   #define UINT16_C(N) N

   typedef int int32_t;
   typedef unsigned int uint32_t;
   #define INT32_C(N) N
   #define UINT32_C(N) N ## U

   typedef long long int int64_t;
   typedef unsigned long long int uint64_t;
   #define INT64_C(N) N ## LL
   #define UINT64_C(N) N ## ULL

   typedef uint32_t uintptr_t;
   #define UINTPTR_MAX UINT32_C(0xffffffff)
#elif defined(__LLP64__)
   typedef signed char int8_t;
   typedef unsigned char uint8_t;
   #define INT8_C(N) N
   #define UINT8_C(N) N

   typedef signed short int int16_t;
   typedef unsigned short int uint16_t;
   #define INT16_C(N) N
   #define UINT16_C(N) N

   typedef int int32_t;
   typedef unsigned int uint32_t;
   #define INT32_C(N) N
   #define UINT32_C(N) N ## U

   typedef long long int int64_t;
   typedef unsigned long long int uint64_t;
   #define INT64_C(N) N ## LL
   #define UINT64_C(N) N ## ULL

   typedef uint64_t uintptr_t;
   #define UINTPTR_MAX UINT64_C(0xffffffffffffffff)
#elif defined(__LP64__)
   typedef signed char int8_t;
   typedef unsigned char uint8_t;
   #define INT8_C(N) N
   #define UINT8_C(N) N

   typedef signed short int int16_t;
   typedef unsigned short int uint16_t;
   #define INT16_C(N) N
   #define UINT16_C(N) N

   typedef int int32_t;
   typedef unsigned int uint32_t;
   #define INT32_C(N) N
   #define UINT32_C(N) N ## U

   typedef long int int64_t;
   typedef unsigned long int uint64_t;
   #define INT64_C(N) N ## L
   #define UINT64_C(N) N ## UL

   typedef uint64_t uintptr_t;
   #define UINTPTR_MAX UINT64_C(0xffffffffffffffff)
#elif defined(__ILP64__)
   typedef signed char int8_t;
   typedef unsigned char uint8_t;
   #define INT8_C(N) N
   #define UINT8_C(N) N

   typedef signed short int int16_t;
   typedef unsigned short int uint16_t;
   #define INT16_C(N) N
   #define UINT16_C(N) N

   typedef int int64_t;
   typedef unsigned int uint64_t;
   #define INT64_C(N) N
   #define UINT64_C(N) N ## U

   typedef uint64_t uintptr_t;
   #define UINTPTR_MAX UINT64_C(0xffffffffffffffff)
#endif // defined(__LP32__)




#if defined(UINT8_C)
   #define INT8_MIN  (-INT8_C(0x7f)-INT8_C(0x01))
   #define INT8_MAX  INT8_C(0x7f)
   #define UINT8_MAX  UINT8_C(0xff)
   typedef int8_t int_least8_t;
   typedef uint8_t uint_least8_t;
   #define INTLEAST8_MIN INT8_MIN
   #define INTLEAST8_MAX INT8_MAX
   #define UINTLEAST8_MAX UINT8_MAX
#else
   typedef signed char int_least8_t;
   typedef unsigned char uint_least8_t;
   #define INTLEAST8_MIN (-0x7f-0x1)
   #define INTLEAST8_MAX 0x7f
   #define UINTLEAST8_MAX 0xff
   #define INT8_C(N) N
   #define UINT8_C(N) N
#endif // defined(UINT8_C)

#if defined(UINT16_C)
   #define INT16_MIN (-INT16_C(0x7fff)-INT16_C(0x0001))
   #define INT16_MAX INT16_C(0x7fff)
   #define UINT16_MAX UINT16_C(0xffff)
   typedef int16_t int_least16_t;
   typedef uint16_t uint_least16_t;
   #define INTLEAST16_MIN INT16_MIN
   #define INTLEAST16_MAX INT16_MAX
   #define UINTLEAST16_MAX UINT16_MAX
#else
   typedef signed short int_least16_t;
   typedef unsigned short uint_least16_t;
   #define INTLEAST16_MIN (-0x7fff-0x1)
   #define INTLEAST16_MAX 0x7fff
   #define UINTLEAST16_MAX 0xffffU
   #define INT16_C(N) N
   #define UINT16_C(N) N ## U
#endif // defined(UINT16_C)

#if defined(UINT32_C)
   #define INT32_MIN (-INT32_C(0x7fffffff)-INT32_C(0x00000001))
   #define INT32_MAX INT32_C(0x7fffffff)
   #define UINT32_MAX UINT32_C(0xffffffff)
   typedef int32_t int_least32_t;
   typedef uint32_t uint_least32_t;
   #define INTLEAST32_MIN INT32_MIN
   #define INTLEAST32_MAX INT32_MAX
   #define UINTLEAST32_MAX UINT32_MAX
#else
   typedef long int int_least32_t;
   typedef unsigned long int uint_least32_t;
   #define INTLEAST32_MIN (-0x7fffffffL-0x1L)
   #define INTLEAST32_MAX 0x7fffffffL
   #define UINTLEAST32_MAX 0xffffffffUL
   #define INT32_C(N) N ## L
   #define UINT32_C(N) N ## UL
#endif // defined(UINT32_C)

#if defined(UINT64_C)
   #define INT64_MIN (-INT64_C(0x7fffffffffffffff)-INT64_C(0x0000000000000001))
   #define INT64_MAX INT64_C(0x7fffffffffffffff)
   #define UINT64_MAX UINT64_C(0xffffffffffffffff)
   typedef int64_t int_least64_t;
   typedef uint64_t uint_least64_t;
   #define INTLEAST64_MIN INT64_MIN
   #define INTLEAST64_MAX INT64_MAX
   #define UINTLEAST64_MAX UINT64_MAX
#else
   typedef long long int int_least64_t;
   typedef unsigned long long int uint_least64_t;
   #define INTLEAST64_MIN (-0x7fffffffffffffffLL-0x1LL)
   #define INTLEAST64_MAX 0x7fffffffLL
   #define UINTLEAST64_MAX 0xffffffffULL
   #define INT64_C(N) N ## LL
   #define UINT64_C(N) N ## ULL
#endif // defined(UINT64_C)




#if defined(UINTPTR_MAX)
#if UINTPTR_MAX == UINT32_C(0xffffffff)
   typedef int32_t intptr_t;
   #define INTPTR_MIN INT32_MIN
   #define INTPTR_MAX INT32_MAX
#elif UINTPTR_MAX == UINT64_C(0xffffffffffffffff)
   typedef int64_t intptr_t;
   #define INTPTR_MIN INT64_MIN
   #define INTPTR_MAX INT64_MAX
#endif // UINTPTR_MAX == UINT32_C(0xffffffff)
#endif // defined(UINTPTR_MAX)

// FIXME: From here it is only a scratch

#define INTFAST8_MIN  0xff
#define INTFAST16_MIN 0xffff
#define INTFAST32_MIN 0xffffffff
#define INTFAST64_MIN 0xffffffffffffffff

#define INTFAST8_MAX  0x7f
#define INTFAST16_MAX 0x7fff
#define INTFAST32_MAX 0x7fffffff
#define INTFAST64_MAX 0x7fffffffffffffff

#define UINTFAST8_MAX  0xffU
#define UINTFAST16_MAX 0xffffU
#define UINTFAST32_MAX 0xffffffffU
#define UINTFAST64_MAX 0xffffffffffffffffU

#define INTMAX_MIN
#define INTMAX_MIN

#if __STDC_VERSION__ >= 202311L

   #define INT8_WIDTH
   #define INT16_WIDTH
   #define INT32_WIDTH
   #define INT64_WIDTH

   #define INT_FAST8_WIDTH
   #define INT_FAST16_WIDTH
   #define INT_FAST32_WIDTH
   #define INT_FAST64_WIDTH

   #define INT_LEAST8_WIDTH
   #define INT_LEAST16_WIDTH
   #define INT_LEAST32_WIDTH
   #define INT_LEAST64_WIDTH

   #define INTPTR_WIDTH
   #define INTMAX_WIDTH

   #define UINT8_WIDTH
   #define UINT16_WIDTH
   #define UINT32_WIDTH
   #define UINT64_WIDTH

   #define UINT_FAST8_WIDTH
   #define UINT_FAST16_WIDTH
   #define UINT_FAST32_WIDTH
   #define UINT_FAST64_WIDTH

   #define UINT_LEAST8_WIDTH
   #define UINT_LEAST16_WIDTH
   #define UINT_LEAST32_WIDTH
   #define UINT_LEAST64_WIDTH

   #define UINTPTR_WIDTH
   #define UINTMAX_WIDTH

#endif /* __STDC_VERSION__ >= 202311L */

typedef void int_fast8_t;
typedef void int_fast16_t;
typedef void int_fast32_t;
typedef void int_fast64_t;

typedef int64_t intmax_t;

typedef void uint_fast8_t;
typedef void uint_fast16_t;
typedef void uint_fast32_t;
typedef void uint_fast64_t;

typedef uint64_t uintmax_t;

/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
#endif /* __STDC_VERSION__ >= 199901L */
#endif /* defined(__STDC_VERSION__) */
#ifdef __cplusplus
}
#endif
#endif /* _STDINT_H_ */
