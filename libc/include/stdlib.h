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
#ifndef _STDLIB_H_
#define _STDLIB_H_
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__STDC__)
/* *****************************************************************************
   Standard header <stdlib.h>.
   Last modified: Jule 16, 2024.
***************************************************************************** */

/* stddef.h */
typedef typeof(sizeof(0)) size_t;

/* wchar.h */
typedef unsigned short wchar_t;

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 255
#define RAND_MAX (0x7fffffff)

int atoi (const char *);
long atol (const char *);
long long atoll (const char *);
double atof (const char *);

float strtof (const char * restrict, char ** restrict);
double strtod (const char * restrict, char ** restrict);
long double strtold (const char * restrict, char ** restrict);

long strtol (const char * restrict, char ** restrict, int);
unsigned long strtoul (const char * restrict, char ** restrict, int);
long long strtoll (const char * restrict, char ** restrict, int);
unsigned long long strtoull (const char * restrict, char ** restrict, int);

int rand (void);
void srand (unsigned);

void *malloc(size_t size);
void *calloc (size_t, size_t);
void *realloc (void *, size_t);
void free(void *);
void *aligned_alloc(size_t alignment, size_t size);
size_t memalignment(const void *p);

void __mprotect(void *ptr, size_t size, int prot);

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ < 201112L
   void abort(void);
   void exit(int exit_code);
#elif __STDC_VERSION__ < 202311L
   _Noreturn void abort(void);
   _Noreturn void exit(int exit_code);
   _Noreturn void quick_exit(int exit_code);
#else
   [[noreturn]] void abort(void);
   [[noreturn]] void exit(int exit_code);
   [[noreturn]] void quick_exit(int exit_code);
#endif /* __STDC_VERSION__ < 201112L */

#if __STDC_VERSION__ >= 199901L

#endif /* __STDC_VERSION__ >= 199901L */
#else
   void abort(void);
   void exit(int exit_code);
#endif /* defined(__STDC_VERSION__) */



int atexit(void(*function)(void));
int at_quick_exit(void (*function)(void));

char *getenv(const char *);
int system( const char *command );

void qsort(void *, size_t, size_t, int (*)(const void *, const void *));

int abs(int);
long labs(long);
long long llabs(long long);

typedef struct { int quot, rem; } div_t;
typedef struct { long quot, rem; } ldiv_t;
typedef struct { long long quot, rem; } lldiv_t;

div_t div (int, int);
ldiv_t ldiv (long, long);
lldiv_t lldiv (long long, long long);

int mblen (const char *, size_t);
int mbtowc (wchar_t *__restrict, const char *__restrict, size_t);
int wctomb (char *, wchar_t);
size_t mbstowcs (wchar_t *__restrict, const char *__restrict, size_t);
size_t wcstombs (char *__restrict, const wchar_t *__restrict, size_t);

/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
#endif /* defined(__STDC__) */
#ifdef __cplusplus
}
#endif
#endif /* _STDLIB_H_ */
