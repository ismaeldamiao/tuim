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
#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------
   Standard aligned_alloc for old compilers.
   * Part of tuim project.
   * Last modified: Octubre 14, 2024.
------------------------------------ */

#ifndef STATIC
#define STATIC
#endif

#if defined(__ANDROID_API__)
   #if __ANDROID_API__ < 28
      STATIC void* aligned_alloc(size_t alignment, size_t size){
         void *buf;
         if(posix_memalign(&buf, alignment, size) == 0)
            return buf;
         else
            return NULL;
      }
   #endif /* __ANDROID_API__ < 28 */
#elif __STDC_VERSION__ < 201112L
   #if defined(_MSC_VER)
      #include <malloc.h>
      /* FIXME: You can get a error if use free(). */
      STATIC void* aligned_alloc(size_t alignment, size_t size){
         return _aligned_malloc(size, alignment);
      }
   #else
      #error "aligned_alloc is needed but implemented."
   #endif /* defined(_MSC_VER) */
#endif /* __ANDROID_API__ */
