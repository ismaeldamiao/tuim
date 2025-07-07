/* *****************************************************************************
   MIT License

   Copyright (c) 2023 I.F.F. dos SANTOS

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
#ifndef _INTTYPES_H_
#define _INTTYPES_H_
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 199901L
/* *****************************************************************************
   Standard header <inttypes.h>.
   Last modified: Jule 16, 2024.
***************************************************************************** */


// TODO: Complicated...

#define PRIdMAX /* Implementation defined */
#define PRIiMAX /* Implementation defined */
#define PRIoMAX /* Implementation defined */
#define PRIuMAX /* Implementation defined */
#define PRIxMAX /* Implementation defined */
#define PRIXMAX /* Implementation defined */
#define SCNdMAX /* Implementation defined */
#define SCNiMAX /* Implementation defined */
#define SCNoMAX /* Implementation defined */
#define SCNuMAX /* Implementation defined */
#define SCNxMAX /* Implementation defined */

#define PRIdPTR /* Implementation defined */
#define PRIiPTR /* Implementation defined */
#define PRIoPTR /* Implementation defined */
#define PRIuPTR /* Implementation defined */
#define PRIxPTR /* Implementation defined */
#define PRIXPTR /* Implementation defined */
#define SCNdPTR /* Implementation defined */
#define SCNiPTR /* Implementation defined */
#define SCNoPTR /* Implementation defined */
#define SCNuPTR /* Implementation defined */
#define SCNxPTR /* Implementation defined */

#if __STDC_VERSION__ >= 202311L
   typedef struct __imaxdiv_t imaxdiv_t;

   intmax_t imaxabs(intmax_t j);
   imaxdiv_t imaxdiv(intmax_t numer, intmax_t denom);
   intmax_t strtoimax(const char * restrict nptr, char ** restrict endptr, int base);
   uintmax_t strtoumax(const char * restrict nptr, char ** restrict endptr, int base);
   intmax_t wcstoimax(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base);
   uintmax_t wcstoumax(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base);
#endif // __STDC_VERSION__ >= 202311L

/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
#endif // __STDC_VERSION__ >= 199901L
#endif // defined(__STDC_VERSION__)
#ifdef __cplusplus
}
#endif
#endif // _INTTYPES_H_
