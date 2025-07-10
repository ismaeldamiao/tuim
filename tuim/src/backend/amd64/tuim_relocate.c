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
#include <stddef.h>
#include <stdint.h>
#include "elf.h"
#include "tuim_backend.h"
/* ------------------------------------
   Relocate EM_X86_64 files.
   * Reference document:
     System V Application Binary Interface
     AMD64 Architecture Processor Supplement
     (With LP64 and ILP32 Programming Models)
     Version 1.0
     https://gitlab.com/x86-psABIs/x86-64-ABI
   * Part of Tuim Project.
   * Last modified: July 10, 2025.
------------------------------------ */

#define USE_ELF32_TEMPLATE
#include "relocate.c"
#undef Elf
#undef RELOCATE
#undef ELF32_Rel_addend
#undef Swap_Addr
#undef USE_ELF32_TEMPLATE

#define USE_ELF64_TEMPLATE
#include "relocate.c"
#undef USE_ELF64_TEMPLATE


const Elf32_Byte *
tuim_relocate(const tuim_ctx *ctx, void *ptr,const void *_rel,const void *_dep){

   const Elf(Byte) * const obj = ((struct tuim_backend*)ptr)->obj;

   if(obj[EI_CLASS] == ELFCLASS32)
      return relocate32(ctx, ptr, _rel, _dep);
   else if(obj[EI_CLASS] == ELFCLASS64)
      return relocate64(ctx, ptr, _rel, _dep);

   return obj + EI_CLASS;
}
