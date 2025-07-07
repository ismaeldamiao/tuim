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
#ifndef _LIMITS_H_
#define _LIMITS_H_
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__STDC__)
/* *****************************************************************************
   Standard header <limits.h>.
   Last modified: Jule 17, 2024.
***************************************************************************** */

#define MB_LEN_MAX           __MB_LEN_MAX // ???

/* Start: Choosing data model. ---------------------------------------------- */

#if defined(__LP32__)
   #define __CHAR_WIDTH 8
   #define __USHRT_WIDTH 16
   #define __UINT_WIDTH 16
   #define __ULONG_WIDTH 32
   #define __ULLONG_WIDTH 64
#elif defined(__ILP32__)
   #define __CHAR_WIDTH 8
   #define __USHRT_WIDTH 16
   #define __UINT_WIDTH 32
   #define __ULONG_WIDTH 32
   #define __ULLONG_WIDTH 64
#elif defined(__LLP64__)
   #define __CHAR_WIDTH 8
   #define __USHRT_WIDTH 16
   #define __UINT_WIDTH 32
   #define __ULONG_WIDTH 32
   #define __ULLONG_WIDTH 64
#elif defined(__LP64__)
   #define __CHAR_WIDTH 8
   #define __USHRT_WIDTH 16
   #define __UINT_WIDTH 32
   #define __ULONG_WIDTH 64
   #define __ULLONG_WIDTH 64
#elif defined(__ILP64__)
   #define __CHAR_WIDTH 8
   #define __USHRT_WIDTH 16
   #define __UINT_WIDTH 64
   #define __ULONG_WIDTH 64
   #define __ULLONG_WIDTH 64
#else
   #error "Unknown data model."
#endif
/* End: Choosing data model. ------------------------------------------------ */

#define CHAR_BIT             __CHAR_WIDTH
#if __CHAR_WIDTH == 8
   #define SCHAR_MIN         (-0x7f-0x01)
   #define SCHAR_MAX         0x7f
   #define UCHAR_MAX         0xff
   #define CHAR_MIN          SCHAR_MIN
   #define CHAR_MAX          SCHAR_MAX
#endif /* __CHAR_WIDTH == 8 */

#if __USHRT_WIDTH == 16
   #define SHRT_MIN          (-0x7fff-0x0001)
   #define SHRT_MAX          0x7fff
   #define USHRT_MAX         0xffff
#endif /* __USHRT_WIDTH == 16 */

#if __UINT_WIDTH == 16
   #define INT_MIN           (-0x7fff-0x0001)
   #define INT_MAX           0x7fff
   #define UINT_MAX          0xffffU
#elif __UINT_WIDTH == 32
   #define INT_MIN           (-0x7fffffff-0x00000001)
   #define INT_MAX           0x7fffffff
   #define UINT_MAX          0xffffffffU
#elif __UINT_WIDTH == 64
   #define INT_MIN           (-0x7fffffffffffffff-0x0000000000000001)
   #define INT_MAX           0x7fffffffffffffff
   #define UINT_MAX          0xffffffffffffffffU
#endif /* __UINT_WIDTH == 16 */

#if __ULONG_WIDTH == 32
   #define LONG_MIN           (-0x7fffffffL-0x00000001L)
   #define LONG_MAX           0x7fffffffL
   #define ULONG_MAX          0xffffffffUL
#elif __ULONG_WIDTH == 64
   #define LONG_MIN          (-0x7fffffffffffffffL-0x0000000000000001L)
   #define LONG_MAX          0x7fffffffffffffffL
   #define ULONG_MAX         0xffffffffffffffffUL
#endif /* __ULONG_WIDTH == 32 */

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 199901L
#if __ULLONG_WIDTH == 64
   #define LLONG_MIN         (-0x7fffffffffffffffLL-0x0000000000000001LL)
   #define LLONG_MAX         0x7fffffffffffffffLL
   #define ULLONG_MA         0xffffffffffffffffULL
#endif /* __ULLONG_WIDTH == 64 */
#endif /* __STDC_VERSION__ >= 199901L */
#if __STDC_VERSION__ >= 202311L
   #define BOOL_WIDTH        __BOOL_WIDTH
   #define CHAR_WIDTH        __CHAR_WIDTH
   #define USHRT_WIDTH       __USHRT_WIDTH
   #define UINT_WIDTH        __UINT_WIDTH
   #define ULONG_WIDTH       __ULONG_WIDTH
   #define ULLONG_WIDTH      __ULLONG_WIDTH

   #define SCHAR_WIDTH       CHAR_BIT
   #define UCHAR_WIDTH       CHAR_BIT
   #define SHRT_WIDTH        USHRT_WIDTH
   #define INT_WIDTH         UINT_WIDTH
   #define LONG_WIDTH        ULONG_WIDTH
   #define LLONG_WIDTH       ULLONG_WIDTH
   #define BITINT_MAXWIDTH   ULLONG_WIDTH // ???
#endif /* __STDC_VERSION__ >= 202311L */
#endif /* defined(__STDC_VERSION__) */

/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
#endif /* defined(__STDC__) */
#ifdef __cplusplus
}
#endif
#endif /* _LIMITS_H_ */
