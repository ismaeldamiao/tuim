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
#ifndef TUIM_PRIVATE_H
#define TUIM_PRIVATE_H 1
#include <stdint.h>
/* ------------------------------------
   Definition of incomplete types on <tuim.h>.
   * Part of tuim project.
   * Last modified: January 13, 2025.
------------------------------------ */

struct tuim_elf {
   void *ehdr;   /* Pointer to ELF header */
   void *phdr;   /* Program header array */
   void *shdr;   /* Section header array */
   void **segments; /* Array of pointers to segments */
   void **sections; /* Array of pointers to sections */
   void *program; /* Program memory image (value is the load address) */
   uintptr_t start_vaddr; /* Lowest virtual address of a PT_LOAD segment. */
   /* Note that the base address is: (uintptr_t)program - start_vaddr */
};

#endif /* TUIM_PRIVATE_H */
