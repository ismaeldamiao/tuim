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
   This function return a relocation entry.
   * Part of Tuim Project.
   * Last modified: July 07, 2025.
------------------------------------ */

#if TUIM_BUILD_FLAGS & TUIM_BF_ELF32
   #define USE_ELF32_TEMPLATE
   #include "templates/get_relocation.c"
   #undef USE_ELF32_TEMPLATE
#endif

#if TUIM_BUILD_FLAGS & TUIM_BF_ELF64
   #define USE_ELF64_TEMPLATE
   #include "templates/get_relocation.c"
   #undef USE_ELF64_TEMPLATE
#endif

const void* tuim_get_relocation(const tuim_ctx *ctx, void *ptr){
   struct tuim_backend *info = ptr;
   void *next;

   (void)ctx;

   if(info->auxiliary == NULL){

      if(info->dyn == NULL) return NULL;

      /* call the correct implementation of read_dynamic_table */

      #if (TUIM_BUILD_FLAGS & TUIM_BF_ELF32) && (TUIM_BUILD_FLAGS & TUIM_BF_ELF64)
         if(info->obj[EI_CLASS] == ELFCLASS32)
            read_dynamic_table32(info);
         else if(info->obj[EI_CLASS] == ELFCLASS64)
            read_dynamic_table64(info);
         return NULL;
      #elif TUIM_BUILD_FLAGS & TUIM_BF_ELF32
         read_dynamic_table32(info);
      #elif TUIM_BUILD_FLAGS & TUIM_BF_ELF64
         read_dynamic_table64(info);
      #endif

      if(info->pltrel != DT_NULL){
         info->is_rel =  (info->pltrel == DT_REL);
         info->is_rela =  (info->pltrel == DT_RELA);
         info->auxiliary = info->jmprel;
      }else{
         info->is_rel = (info->rel != NULL);
         info->is_rela = (!(info->is_rel) && (info->rela != NULL));
         info->auxiliary = ((info->rel != NULL) ? info->rel : info->rela);
      }
      return info->auxiliary;
   }

   if(info->jmprel != NULL){
      next = (uint8_t*)(info->auxiliary) + (
         (info->pltrel == DT_REL) ? info->relent : info->relaent);
      if(next != (uint8_t*)(info->jmprel) + info->pltrelsz){
         info->auxiliary = next;
      }else{
         info->jmprel = NULL;

         info->is_rel = (info->rel != NULL);
         info->is_rela = (!(info->is_rel) && (info->rela != NULL));
         info->auxiliary = ((info->rel != NULL) ? info->rel : info->rela);
      }
   }else if(info->rel != NULL){
      next = (uint8_t*)(info->auxiliary) + info->relent;
      if(next != (uint8_t*)(info->rel) + info->relsz){
         info->auxiliary = next;
      }else{
         info->rel = NULL;

         info->is_rel = false;
         info->is_rela = (info->rela != NULL);
         info->auxiliary = info->rela;
      }
   }else if(info->rela != NULL){
      next = (uint8_t*)(info->auxiliary) + info->relaent;
      if(next != (uint8_t*)(info->rela) + info->relasz){
         info->auxiliary = next;
      }else{
         info->rela = NULL;

         info->is_rela = false;
         info->auxiliary = NULL;
      }
   }

   return info->auxiliary;
}
