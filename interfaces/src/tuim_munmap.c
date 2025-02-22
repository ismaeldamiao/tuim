/* *****************************************************************************
   MIT License

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
#include "main.h"
/* ------------------------------------
   This is function release memory addresses allocated by the tuim_mmap.
   * Part of Tuim Project.
   * Last modified: February 21, 2025.
------------------------------------ */

#if defined(_POSIX_C_SOURCE)
#include <sys/mman.h>
#else
#error Unknown target OS
#endif /* defined(_POSIX_C_SOURCE) */

void tuim_munmap(struct file_s *file, struct image_s *image){
#if defined(_POSIX_C_SOURCE)
   if(file != NULL)
      munmap(file->obj, file->sz);
   if(image != NULL)
      munmap(image->program, image->sz);
#elif defined(_WIN32)
   if(file != NULL)
      UnmapViewOfFile(file->obj);
   if(image != NULL)
      VirtualFree(image->program, 0, MEM_RELEASE);
#else
#error Unknown target OS
#endif /* defined(_POSIX_C_SOURCE) */
}
