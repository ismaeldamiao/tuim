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
   This function return a relocation entry.
   * Part of Tuim Project.
   * Last modified: July 15, 2025.
------------------------------------ */
#include "../tuim_impl.h"

#if TUIM_BUILD_FLAGS & TUIM_BF_ELF32
   #define USE_ELF32_TEMPLATE
   #include "get_relocation.c"
   #undef USE_ELF32_TEMPLATE
#endif

#if TUIM_BUILD_FLAGS & TUIM_BF_ELF64
   #define USE_ELF64_TEMPLATE
   #include "get_relocation.c"
   #undef USE_ELF64_TEMPLATE
#endif

const void* tuim_get_relocation(tuim_elf *elf){
   #if (TUIM_BUILD_FLAGS & TUIM_BF_ELF32) && (TUIM_BUILD_FLAGS & TUIM_BF_ELF64)
      const uint8_t *obj = elf->file.obj;
   #endif
   void *next;

   if(elf->auxiliary == NULL){

      if(elf->dyn == NULL) return NULL;

      /* call the correct implementation of read_dynamic_table */

      #if (TUIM_BUILD_FLAGS & TUIM_BF_ELF32) && (TUIM_BUILD_FLAGS & TUIM_BF_ELF64)
         if(obj[EI_CLASS] == ELFCLASS32)
            read_dynamic_table32(elf);
         else if(obj[EI_CLASS] == ELFCLASS64)
            read_dynamic_table64(elf);
         return NULL;
      #elif TUIM_BUILD_FLAGS & TUIM_BF_ELF32
         read_dynamic_table32(elf);
      #elif TUIM_BUILD_FLAGS & TUIM_BF_ELF64
         read_dynamic_table64(elf);
      #endif

      if(elf->pltrel != DT_NULL){
         elf->is_rel =  (elf->pltrel == DT_REL);
         elf->is_rela =  (elf->pltrel == DT_RELA);
         elf->auxiliary = elf->jmprel;
      }else{
         elf->is_rel = (elf->rel != NULL);
         elf->is_rela = (!(elf->is_rel) && (elf->rela != NULL));
         elf->auxiliary = ((elf->rel != NULL) ? elf->rel : elf->rela);
      }
      return elf->auxiliary;
   }

   if(elf->jmprel != NULL){
      next = (uint8_t*)(elf->auxiliary) + (
         (elf->pltrel == DT_REL) ? elf->relent : elf->relaent);
      if(next != (uint8_t*)(elf->jmprel) + elf->pltrelsz){
         elf->auxiliary = next;
      }else{
         elf->jmprel = NULL;

         elf->is_rel = (elf->rel != NULL);
         elf->is_rela = (!(elf->is_rel) && (elf->rela != NULL));
         elf->auxiliary = ((elf->rel != NULL) ? elf->rel : elf->rela);
      }
   }else if(elf->rel != NULL){
      next = (uint8_t*)(elf->auxiliary) + elf->relent;
      if(next != (uint8_t*)(elf->rel) + elf->relsz){
         elf->auxiliary = next;
      }else{
         elf->rel = NULL;

         elf->is_rel = false;
         elf->is_rela = (elf->rela != NULL);
         elf->auxiliary = elf->rela;
      }
   }else if(elf->rela != NULL){
      next = (uint8_t*)(elf->auxiliary) + elf->relaent;
      if(next != (uint8_t*)(elf->rela) + elf->relasz){
         elf->auxiliary = next;
      }else{
         elf->rela = NULL;

         elf->is_rela = false;
         elf->auxiliary = NULL;
      }
   }

   return elf->auxiliary;
}
