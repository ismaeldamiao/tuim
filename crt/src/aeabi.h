/* *****************************************************************************
   MIT License

   Copyright (c) 2024 I.F.F. dos Santos <ismaellxd@gmail.com>

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
#ifndef __AEABI_H__
#define __AEABI_H__
#ifdef __cplusplus
extern "C" {
#endif
/* *****************************************************************************
   Run-time ABI for the Arm Architecture.
   * In general, better do not use this header.
   Last modified: December 12, 2024.
***************************************************************************** */

#define __value_in_regs
typedef struct lldiv_s  { long long int quot, rem;  } lldiv_t;
typedef struct ulldiv_s { unsigned long long int quot, rem; } ulldiv_t;

typedef struct { int quot; int rem; } idiv_return;
typedef struct { unsigned quot; unsigned rem; } uidiv_return;

#define float  unsigned int
#define double unsigned long long int
#define size_t unsigned int

/* ---
5.1   Floating-point library
--- */

double __aeabi_dadd(double, double);
double __aeabi_ddiv(double n, double d);
double __aeabi_dmul(double, double);
double __aeabi_drsub(double x, double y);
double __aeabi_dsub(double x, double y);

void __aeabi_cdcmpeq(double, double);
void __aeabi_cdcmple(double, double);
void __aeabi_cdrcmple(double, double);
int __aeabi_dcmpeq(double, double);
int __aeabi_dcmplt(double, double);
int __aeabi_dcmple(double, double);
int __aeabi_dcmpge(double, double);
int __aeabi_dcmpgt(double, double);
int __aeabi_dcmpun(double, double);

float __aeabi_fadd(float, float);
float __aeabi_fdiv(float n, float d);
float __aeabi_fmul(float, float);
float __aeabi_frsub(float x, float y);
float __aeabi_fsub(float x, float y);

void __aeabi_cfcmpeq(float, float);
void __aeabi_cfcmple(float, float);
void __aeabi_cfrcmple(float, float);
int __aeabi_fcmpeq(float, float);
int __aeabi_fcmplt(float, float);
int __aeabi_fcmple(float, float);
int __aeabi_fcmpge(float, float);
int __aeabi_fcmpgt(float, float);
int __aeabi_fcmpun(float, float);

int __aeabi_d2iz(double);
unsigned __aeabi_d2uiz(double);
long long __aeabi_d2lz(double);
unsigned long long __aeabi_d2ulz(double);
int __aeabi_f2iz(float);
unsigned __aeabi_f2uiz(float);
long long __aeabi_f2lz(float);
unsigned long long __aeabi_f2ulz(float);

float __aeabi_d2f(double);
double __aeabi_f2d(float);
float __aeabi_h2f(short hf);
float __aeabi_h2f_alt(short hf);
short __aeabi_f2h(float f);
short __aeabi_f2h_alt(float f);
short __aeabi_d2h(double);
short __aeabi_d2h_alt(double);

double __aeabi_i2d(int);
double __aeabi_ui2d(unsigned);
double __aeabi_l2d(long long);
double __aeabi_ul2d(unsigned long long);
float __aeabi_i2f(int);
float __aeabi_ui2f(unsigned);
float __aeabi_l2f(long long);
float __aeabi_ul2f(unsigned long long);

/* ---
5.2   The long long helper functions
--- */

long long __aeabi_lmul(long long, long long);
long long __aeabi_llsl(long long a, int shift);
long long __aeabi_llsr(long long a, int shift);
long long __aeabi_lasr(long long, int);

int __aeabi_lcmp(long long, long long);
int __aeabi_ulcmp(unsigned long long, unsigned long long);

__value_in_regs lldiv_t __aeabi_ldivmod(long long n, long long d);
__value_in_regs ulldiv_t __aeabi_uldivmod(unsigned long long n, unsigned long long d);

/* ---
5.3   Other C and assembly language helper functions
--- */

/* 5.3.1   Integer (32/32 -> 32) division functions */

int __aeabi_idiv(int numerator, int denominator);
unsigned __aeabi_uidiv(unsigned numerator, unsigned denominator);

__value_in_regs
idiv_return __aeabi_idivmod(int numerator, int denominator);
__value_in_regs
uidiv_return __aeabi_uidivmod(unsigned numerator, unsigned denominator);

/* 5.3.2   Division by zero */

int __aeabi_idiv0(int return_value);
long long __aeabi_ldiv0(long long return_value);

/* 5.3.3   Unaligned memory access */

int __aeabi_uread4(void *address);
int __aeabi_uwrite4(int value, void *address);
long long __aeabi_uread8(void *address);
long long __aeabi_uwrite8(long long value, void *address);

/* 5.3.4   Memory copying, clearing, and setting */

void __aeabi_memcpy8(void *dest, const void *src, size_t n);
void __aeabi_memcpy4(void *dest, const void *src, size_t n);
void __aeabi_memcpy(void *dest, const void *src, size_t n);

void __aeabi_memmove8(void *dest, const void *src, size_t n);
void __aeabi_memmove4(void *dest, const void *src, size_t n);
void __aeabi_memmove(void *dest, const void *src, size_t n);

void __aeabi_memset8(void *dest, size_t n, int c);
void __aeabi_memset4(void *dest, size_t n, int c);
void __aeabi_memset(void *dest, size_t n, int c);

void __aeabi_memclr8(void *dest, size_t n);
void __aeabi_memclr4(void *dest, size_t n);
void __aeabi_memclr(void *dest, size_t n);

/* 5.3.5   Thread-local storage (new in v2.01) */

void *__aeabi_read_tp(void);

/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
#ifdef __cplusplus
}
#endif
#endif /* __AEABI_H__ */
