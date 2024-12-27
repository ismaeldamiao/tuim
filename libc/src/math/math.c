 /* *****************************************************************************
   MIT License

   Copyright (c) 2023-2024 I.F.F. dos SANTOS

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



/* *****************************************************************************
   This file include all the source code necessary to have an implementation
   of the libm, i.e., the functions defined in the standard headers:
   - <math.h>
   - <complex.h>
   - <fenv.h>

   COMPILE (any of):
      cc -std=c89 -c libm.c
      cc -std=c99 -c libm.c
      cc -c libm.c
   ARCHIVE (Unix users):
      ar src libm.a libm.o
      rm libm.o
   ************************************************************************** */
#ifdef __STDC__
/* ANSI C, C89 (ANSI X3.159-1989) compatible source code. */
   #include "fabs.c"
   #include "sin.c"
   #include "cos.c"
#endif /* __STDC__ */

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
/* C99 (ISO/IEC 9899:1999) compatible source code. */
#endif /* ISO C99 */

