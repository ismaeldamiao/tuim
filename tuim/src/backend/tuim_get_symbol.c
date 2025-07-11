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
#include "tuim_backend.h"
/* ------------------------------------
   This function find by a symbol definition on dynamic symbol table.
   * Part of Tuim Project.
   * Last modified: July 09, 2025.
------------------------------------ */

#if TUIM_BUILD_FLAGS & TUIM_BF_ELF32
   #define USE_ELF32_TEMPLATE
   #include "templates/get_symbol.c"
   #undef USE_ELF32_TEMPLATE
#endif

#if TUIM_BUILD_FLAGS & TUIM_BF_ELF64
   #define USE_ELF64_TEMPLATE
   #include "templates/get_symbol.c"
   #undef USE_ELF64_TEMPLATE
#endif

const void *tuim_get_sym(void *ptr, const uint8_t *symbol){

   /* call the correct implementation of tuim_get_symbol */

   #if (TUIM_BUILD_FLAGS & TUIM_BF_ELF32) && (TUIM_BUILD_FLAGS & TUIM_BF_ELF64)
      const uint8_t * obj = ((struct tuim_backend *)ptr)->obj;

      if(obj[EI_CLASS] == ELFCLASS32)
         return get_sym32(ptr, symbol);
      else if(obj[EI_CLASS] == ELFCLASS64)
         return get_sym64(ptr, symbol);
      return NULL;
   #elif TUIM_BUILD_FLAGS & TUIM_BF_ELF32
      return get_sym32(ptr, symbol);
   #elif TUIM_BUILD_FLAGS & TUIM_BF_ELF64
      return get_sym64(ptr, symbol);
   #endif
}

uint64_t tuim_get_symbol(const tuim_ctx *ctx, void *ptr, const uint8_t *symbol){
   (void)ctx;

   /* call the correct implementation of tuim_get_symbol */

   #if (TUIM_BUILD_FLAGS & TUIM_BF_ELF32) && (TUIM_BUILD_FLAGS & TUIM_BF_ELF64)
      const uint8_t * obj = ((struct tuim_backend *)ptr)->obj;

      if(obj[EI_CLASS] == ELFCLASS32)
         return get_symbol32(ptr, symbol);
      else if(obj[EI_CLASS] == ELFCLASS64)
         return get_symbol64(ptr, symbol);
      return tuim_nullptr;
   #elif TUIM_BUILD_FLAGS & TUIM_BF_ELF32
      return get_symbol32(ptr, symbol);
   #elif TUIM_BUILD_FLAGS & TUIM_BF_ELF64
      return get_symbol64(ptr, symbol);
   #endif
}
