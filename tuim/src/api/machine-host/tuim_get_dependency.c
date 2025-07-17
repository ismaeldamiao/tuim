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
   * Last modified: July 15, 2025.
------------------------------------ */
#include "../tuim_impl.h"

#if TUIM_BUILD_FLAGS & TUIM_BF_ELF32
   #define USE_ELF32_TEMPLATE
   #include "get_dependency.c"
   #undef USE_ELF32_TEMPLATE
#endif

#if TUIM_BUILD_FLAGS & TUIM_BF_ELF64
   #define USE_ELF64_TEMPLATE
   #include "get_dependency.c"
   #undef USE_ELF64_TEMPLATE
#endif

const uint8_t *tuim_get_dependency(tuim_elf *elf){
   #if (TUIM_BUILD_FLAGS & TUIM_BF_ELF32) && (TUIM_BUILD_FLAGS & TUIM_BF_ELF64)
      const uint8_t *obj = elf->file.obj;
   #endif

   if(elf->auxiliary == NULL){
      if(elf->dyn == NULL) return NULL;
      elf->auxiliary = elf->dyn;
   }

   /* call the correct implementation of tuim_get_dependency */

   #if (TUIM_BUILD_FLAGS & TUIM_BF_ELF32) && (TUIM_BUILD_FLAGS & TUIM_BF_ELF64)
      if(obj[EI_CLASS] == ELFCLASS32)
         return get_dependency32(elf);
      else if(obj[EI_CLASS] == ELFCLASS64)
         return get_dependency64(elf);
      return NULL;
   #elif TUIM_BUILD_FLAGS & TUIM_BF_ELF32
      return get_dependency32(elf);
   #elif TUIM_BUILD_FLAGS & TUIM_BF_ELF64
      return get_dependency64(elf);
   #endif
}
