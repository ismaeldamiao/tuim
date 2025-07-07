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
#ifndef _LOCALE_H_
#define _LOCALE_H_
#ifdef __cplusplus
extern "C" {
#endif
#if defined(__STDC__)
/* *****************************************************************************
   Standard header <locale.h>.
   Last modified: Jule 17, 2024.
***************************************************************************** */

#define LC_ALL               __LC_ALL
#define LC_COLLATE           __LC_COLLATE
#define LC_CTYPE             __LC_CTYPE
#define LC_MONETARY          __LC_MONETARY
#define LC_NUMERIC           __LC_NUMERIC
#define LC_TIME              __LC_TIME

struct lconv {
   char
   /*  Non-monetary numeric formatting parameters */
      *decimal_point,
      *thousands_sep,
      *grouping,
   /* Monetary numeric formatting parameters */
      *mon_decimal_point,
      *mon_thousands_sep,
      *mon_grouping,
      *positive_sign,
      *negative_sign,
   /* Local monetary numeric formatting parameters */
      *currency_symbol,
      frac_digits,
      p_cs_precedes,
      n_cs_precedes,
      p_sep_by_space,
      n_sep_by_space,
      p_sign_posn,
      n_sign_posn,
   /* International monetary numeric formatting parameters */
      *int_curr_symbol,
      int_frac_digits;
   #if defined(__STDC_VERSION__)
   #if __STDC_VERSION__ >= 199901L
   char
      int_p_cs_precedes,
      int_n_cs_precedes,
      int_p_sep_by_space,
      int_n_sep_by_space,
      int_p_sign_posn,
      int_n_sign_posn;
   #endif /* __STDC_VERSION__ >= 199901L */
   #endif /* defined(__STDC_VERSION__) */
};

char* setlocale(int category, const char* locale);
struct lconv *localeconv(void);

/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
#endif /* defined(__STDC__) */
#ifdef __cplusplus
}
#endif
#endif /* _LOCALE_H_ */
