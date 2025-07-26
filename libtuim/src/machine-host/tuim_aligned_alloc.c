/* *****************************************************************************
   Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>

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
#include <stdint.h>
#include <stdlib.h>
/* ------------------------------------
   aligned_alloc implementation for the host machine.
   * Part of Tuim Project.
   * Last modified: July 24, 2025.
------------------------------------ */

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 202311L
#if __has_include(<tuim/kernel.h>)
   #define tuim_environment
#endif // __has_include(<tuim/kernel.h>)
#endif /* __STDC_VERSION__ < 202311L */
#endif /* defined(__STDC_VERSION__) */

#if defined(tuim_environment)
uint64_t tuim_aligned_alloc(uint64_t alignment, uint64_t size){
   uint64_t addr;
   size_t alignment_, size_;

   alignment_ = (size_t)alignment;
   size_ = (size_t)size;
   addr = (uintptr_t)aligned_alloc(alignment_, size_);
   return addr;
}
#else
uint64_t tuim_aligned_alloc(uint64_t alignment, uint64_t size){
   uintptr_t addr;
   size_t alignment_, size_;

   alignment_ = (size_t)alignment;
   size_ = (size_t)size;

   addr = (uintptr_t)malloc(size_ + alignment_ -1);
   addr = (addr + alignment_ -1) & ~(alignment_-1);

   return (uint64_t)addr;
}
#endif /* defined(tuim_environment) */
