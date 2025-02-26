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
#ifndef _STDARG_H_
#define _STDARG_H_
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__STDC__)
/* *****************************************************************************
   Standard header <stdarg.h>.
   Last modified: Jule 16, 2024.
***************************************************************************** */

#if 0
typedef void * va_list;

#if defined(__ARM_ARCH)
#if __ARM_ARCH >= 8
#elif __ARM_ARCH <= 7
   #define va_start(ap, ...)
   #define va_arg(ap, type) (*(type*)0)
   #define va_copy(dest, src)
   #define va_end(ap)
#endif /* __ARM_ARCH >= 8 */
#elif defined(__i386__)
   #define va_start(ap, ...)
   #define va_arg(ap, type) ((ap) = ((char*)(ap) + sizeof(type)))
   #define va_copy(dest, src)
   #define va_end(ap)
#endif /* defined(__ARM_ARCH) */
#endif

typedef __builtin_va_list va_list;
#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
#define va_copy(d,s)    __builtin_va_copy(d,s)


/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
#endif /* defined(__STDC__) */
#ifdef __cplusplus
}
#endif
#endif /* _STDARG_H_ */
