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
#ifndef _SETJMP_H_
#define _SETJMP_H_
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__STDC__)
/* *****************************************************************************
   Standard header <setjmp.h>.
   Last modified: Jule 16, 2024.
***************************************************************************** */

typedef long jmp_buf[14];

int __setjmp(jmp_buf);
#define setjmp(env)          __setjmp(env)

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 202311L
   [[noreturn]] void longjmp(jmp_buf env, int status);
#elif __STDC_VERSION__ >= 201112L
   _Noreturn void longjmp(jmp_buf env, int status);
#else
   void longjmp(jmp_buf env, int status);
#endif /* __STDC_VERSION__ >= 202311L */
#else
   void longjmp(jmp_buf env, int status);
#endif /* defined(__STDC_VERSION__) */


/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
#endif /* defined(__STDC__) */
#ifdef __cplusplus
}
#endif
#endif /* _SETJMP_H_ */
