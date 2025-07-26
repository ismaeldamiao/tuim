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
#ifndef TUIM_KERNEL_H
#define TUIM_KERNEL_H
/* ------------------------------------
   Generic implementation of <tuim/kernel.h> header from The Tuim Interface.

   * Last modified: July 24, 2025.
------------------------------------ */

#define tuim_unknown          0x0000
#define tuim_linux            0x0011
#define tuim_xnu              0x0021
#define tuim_windows_nt       0x0031

#if defined(__linux__)
   #define tuim_kernel        tuim_linux
#else
   #define tuim_kernel        tuim_unknown
#endif

/* Linux system calls ------------------------------------------------------  */
#if   tuim_kernel == tuim_linux || \
      tuim_kernel == tuim_xnu
#include <stddef.h>

typedef long off_t;
typedef long ssize_t_t;

void *mmap(
   void *addr, size_t length, int prot, int flags, int fd, off_t offset
);
int   mprotect(void *addr, size_t size, int prot);
int   munmap(void *addr, size_t length);

int   open(const char *pathname, int flags, .../* mode_t mode */ );
ssize_t_t   write(int fd, const void *buf, size_t len);
off_t lseek(int fildes, off_t offset, int whence);
int   close(int fd);
#endif /* tuim_kernel == tuim_linux */

#endif /* TUIM_KERNEL_H */
