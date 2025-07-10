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
   template for the tuim_get_dependency function
   * Part of Tuim Project.
   * Last modified: July 08, 2025.
------------------------------------ */

#if 0
   #define USE_ELF32_TEMPLATE
#endif

#define Swap_Word  swap32
#define Swap_Xword swap64

#define Swap_Sword  swap32
#define Swap_Sxword swap64

#if defined(USE_ELF32_TEMPLATE)
   #define GET_DEPENDENCY get_dependency32
   #define Elf(x) Elf32_##x
#elif defined(USE_ELF64_TEMPLATE)
   #define GET_DEPENDENCY get_dependency64
   #define Elf(x) Elf64_##x
#endif

//uint8_t *tuim_get_dependency(uint8_t *obj, void **dyn_ptr, uint8_t *dynstr){
static const uint8_t *GET_DEPENDENCY(struct tuim_backend *info){
   const Elf(Byte) *dynstr = info->dynstr;
   const Elf(Dyn) *dyn;
   #if defined(USE_ELF32_TEMPLATE)
      Elf32_Word d_val;
      Elf32_Sword d_tag;
   #elif defined(USE_ELF64_TEMPLATE)
      Elf64_Xword d_val;
      Elf64_Sxword d_tag;
   #endif

   dyn = info->auxiliary;

   #if defined(USE_ELF32_TEMPLATE)
      read_tag: d_tag = Swap_Sword(dyn->d_tag);
   #elif defined(USE_ELF64_TEMPLATE)
      read_tag: d_tag = Swap_Sxword(dyn->d_tag);
   #endif

   if(d_tag == DT_NULL){ info->auxiliary = NULL; return NULL; }
   if(d_tag != DT_NEEDED){ ++dyn; goto read_tag; }

   #if defined(USE_ELF32_TEMPLATE)
      d_val = Swap_Word (dyn->d_un.d_val);
   #elif defined(USE_ELF64_TEMPLATE)
      d_val = Swap_Xword(dyn->d_un.d_val);
   #endif

   info->auxiliary = dyn + 1;
   return dynstr + d_val;
}

#undef GET_DEPENDENCY
#undef Elf
