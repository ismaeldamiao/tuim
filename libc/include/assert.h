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
#if defined(__STDC__)
/* *****************************************************************************
   Standard header <assert.h>.
   Last modified: Jule 17, 2024.
***************************************************************************** */
#if defined(assert)
   #undef assert
#endif


#if defined(NDEBUG)
   #define assert(condition) ((void)0)
#else

   #if defined(__STDC_VERSION__)
   #if __STDC_VERSION__ < 199901L
      #define assert(condition) { if(!(condition)) { \
         void abort(void); int fprintf(struct __FILE*, const char*, ...);\
         fprintf(stderr, \
            __FILE__ ":%d: Assertion '" #condition "' failed.", \
            __LINE__); \
         abort(); \
      }}
   #else
      #define assert(condition) { if(!(condition)) { \
         void abort(void); int fprintf(struct __FILE*, const char*, ...);\
         fprintf(stderr, \
            __FILE__ ":%d: %s: Assertion '" #condition "' failed.", \
            __LINE__, __func__); \
         abort(); \
      }}
   #endif /* __STDC_VERSION__ < 199901L */


   #if __STDC_VERSION__ == 201112L
      #define static_assert _Static_assert
   #elif __STDC_VERSION__ == 202311L
      #define __STDC_VERSION_ASSERT_H__ 202311L
   #endif /* __STDC_VERSION__ == 201112L */
   #endif /* defined(__STDC_VERSION__) */

#endif /* NDEBUG */


/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
#endif /* defined(__STDC__) */
