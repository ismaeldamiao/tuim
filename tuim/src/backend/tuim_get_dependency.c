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
/* ------------------------------------
   This function return a string holding a name of a DT_NEEDED entry on the
   dynamic section.
   * Part of Tuim Project.
   * Last modified: July 07, 2025.
------------------------------------ */
#include "elf.h"
#include "tuim_backend.h"

#if __STDC_VERSION__ < 202311L
   #define true 1
   #define false 0
#endif
#if __STDC_VERSION__ < 199901L
   typedef int bool;
#elif (__STDC_VERSION__ >= 199901L) && (__STDC_VERSION__ < 202311L)
   typedef _Bool bool;
#endif

#ifndef swap16
   #define swap16(x) (x)
#endif

#ifndef swap32
   #define swap32(x) (x)
#endif

#ifndef swap64
   #define swap64(x) (x)
#endif

#ifndef Elf
   #define Elf(x) Elf32_##x
#endif

//uint8_t *tuim_get_dependency(uint8_t *obj, void **dyn_ptr, uint8_t *dynstr){
const uint8_t *tuim_get_dependency(const tuim_ctx *ctx, void *ptr){
   struct tuim_backend *info = ptr;
   const uint8_t *dynstr = info->dynstr;

   bool target_is_32 = (offsetof(Elf(Dyn), d_un) == offsetof(Elf32_Dyn, d_un));

   const Elf(Dyn) *dyn;
   uint64_t d_val;
   int64_t d_tag;

   (void)ctx;

   if(info->auxiliary == NULL){
      if(info->dyn == NULL) return NULL;
      info->auxiliary = info->dyn;
   }

   dyn = info->auxiliary;

   read_tag: d_tag = (target_is_32 ? swap32(dyn->d_tag) : swap64(dyn->d_tag));
   if(d_tag == DT_NULL){ info->auxiliary = NULL; return NULL; }
   if(d_tag != DT_NEEDED){ ++dyn; goto read_tag; }

   d_val = (target_is_32 ? swap32(dyn->d_un.d_val) : swap64(dyn->d_un.d_val));
   info->auxiliary = dyn + 1;
   return dynstr + d_val;
}
