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
#include "tuim_backend.h"
/* ------------------------------------
   Function to load program segments on target machine.
   * Part of Tuim Project.
   * Last modified: July 08, 2025.
------------------------------------ */

#if TUIM_BUILD_FLAGS & TUIM_BF_ELF32
   #define USE_ELF32_TEMPLATE
   #include "templates/load_segments.c"
   #undef Elf
   #undef USE_ELF32_TEMPLATE
#endif

#if TUIM_BUILD_FLAGS & TUIM_BF_ELF64
   #define USE_ELF64_TEMPLATE
   #include "templates/load_segments.c"
   #undef USE_ELF64_TEMPLATE
#endif

int tuim_load_segments(const tuim_ctx *ctx, const uint8_t *obj, void **ptr){
   struct tuim_backend *info;

   /* allocate memory for the backend structure */

   info = malloc(sizeof(struct tuim_backend));
   if(info == NULL){
      return 1;
   }
   *ptr = info;
   info->obj = obj;

   /* call the correct implementation of tuim_load_segment */

   #if (TUIM_BUILD_FLAGS & TUIM_BF_ELF32) && (TUIM_BUILD_FLAGS & TUIM_BF_ELF64)
      if(obj[EI_CLASS] == ELFCLASS32)
         return load_segments32(ctx, obj, info);
      else if(obj[EI_CLASS] == ELFCLASS64)
         return load_segments64(ctx, obj, info);
      return 2;
   #elif TUIM_BUILD_FLAGS & TUIM_BF_ELF32
      return load_segments32(ctx, obj, info);
   #elif TUIM_BUILD_FLAGS & TUIM_BF_ELF64
      return load_segments64(ctx, obj, info);
   #endif
}
