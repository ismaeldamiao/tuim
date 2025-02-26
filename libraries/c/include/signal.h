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
#ifndef _SIGNAL_H_
#define _SIGNAL_H_
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__STDC__)
/* *****************************************************************************
   Standard header <signal.h>.
   Last modified: Jule 17, 2024.
***************************************************************************** */

#define SIGTERM              __SIGTERM
#define SIGSEGV              __SIGSEGV
#define SIGINT               __SIGINT
#define SIGILL               __SIGILL
#define SIGABRT              __SIGABRT
#define SIGFPE               __SIGFPE

#define SIG_DFL              __SIG_DFL
#define SIG_IGN              __SIG_IGN

#define SIG_ERR              __SIG_ERR

typedef /* unspecified */ sig_atomic_t;

void (*signal( int sig, void (*handler) (int))) (int);
int raise( int sig );

/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
#endif /* defined(__STDC__) */
#ifdef __cplusplus
}
#endif
#endif /* _SIGNAL_H_ */