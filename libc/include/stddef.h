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
#ifndef _STDDEF_H_
#define _STDDEF_H_
#ifdef __cplusplus
extern "C" {
#endif
/* *****************************************************************************
   Standard header <stddef.h>.
   Last modified: Octubre 21, 2024.
***************************************************************************** */

#if !defined(__STDC__) || !defined(__STDC_VERSION__)
   #error "You must use a standard compliant compiler."
#else
   #if __STDC_VERSION__ < 202311L
      #error "You must use a C23 standard compliant compiler."
   #endif /* __STDC_VERSION__ < 202311L */
#endif /* !defined(__STDC_VERSION__) */

extern void * const __null;

typedef typeof(sizeof(0)) size_t;
typedef typeof((char*)0-(char*)1) ptrdiff_t;
typedef unsigned long long int max_align_t;
typedef typeof(nullptr) nullptr_t;

#define NULL __null
#define offsetof(type, member) \
   ((size_t)((char *)&((type *)0)->member - (char *)0))

/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
#ifdef __cplusplus
}
#endif
#endif /* _STDDEF_H_ */
