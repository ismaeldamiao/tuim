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
#include <threads.h>
/* ------------------------------------
   This function return a relocation entry.
   * Part of Tuim Project.
   * Last modified: July 07, 2025.
------------------------------------ */
#include "elf.h"

#include "tuim_backend.h"

static void read_dynamic_table(struct tuim_backend *info);

const void* tuim_get_relocation(const tuim_ctx *ctx, void *ptr){
   struct tuim_backend *info = ptr;
   void *next;

   (void)ctx;

   if(info->auxiliary == NULL){
      read_dynamic_table(info);
      if(info->pltrel != DT_NULL){
         info->is_rel =  (info->pltrel == DT_REL);
         info->is_rela =  (info->pltrel == DT_RELA);
         info->auxiliary = info->jmprel;

         if((info->relent == SIZE_C(0)) && (info->pltrel == DT_REL)){
            info->relent = sizeof(Elf(Rel));
         }else if((info->relaent == SIZE_C(0)) && (info->pltrel == DT_RELA)){
            info->relaent = sizeof(Elf(Rela));
         }
      }else{
         info->is_rel = (info->rel != NULL);
         info->is_rela = (!(info->is_rel) && (info->rela != NULL));
         info->auxiliary = ((info->rel != NULL) ? info->rel : info->rela);
      }
      return info->auxiliary;
   }

   if(info->jmprel != NULL){
      next = (Elf(Byte)*)(info->auxiliary) + (
         (info->pltrel == DT_REL) ? info->relent : info->relaent);
      if(next != (Elf(Byte)*)(info->jmprel) + info->pltrelsz){
         info->auxiliary = next;
      }else{
         info->jmprel = NULL;

         info->is_rel = (info->rel != NULL);
         info->is_rela = (!(info->is_rel) && (info->rela != NULL));
         info->auxiliary = ((info->rel != NULL) ? info->rel : info->rela);
      }
   }else if(info->rel != NULL){
      next = (Elf(Byte)*)(info->auxiliary) + info->relent;
      if(next != (Elf(Byte)*)(info->rel) + info->relsz){
         info->auxiliary = next;
      }else{
         info->rel = NULL;

         info->is_rel = false;
         info->is_rela = (info->rela != NULL);
         info->auxiliary = info->rela;
      }
   }else if(info->rela != NULL){
      next = (Elf(Byte)*)(info->auxiliary) + info->relaent;
      if(next != (Elf(Byte)*)(info->rela) + info->relasz){
         info->auxiliary = next;
      }else{
         info->rela = NULL;

         info->is_rela = false;
         info->auxiliary = NULL;
      }
   }

   return info->auxiliary;
}

static void read_dynamic_table(struct tuim_backend *info){
   const Elf(Dyn) *dyn = info->dyn;
   uint64_t d_val, d_ptr;
   int64_t d_tag;

   info->rel = NULL;
   info->rela = NULL;
   info->pltrel = DT_NULL;

   info->relent = info->relaent = SIZE_C(0);

   read_tag: d_tag = (is_Elf32 ? swap32(dyn->d_tag) : swap64(dyn->d_tag));
   if(d_tag == DT_JMPREL){
      d_ptr = (is_Elf32 ? swap32(dyn->d_un.d_ptr) : swap64(dyn->d_un.d_ptr));
      info->jmprel = (void*)(info->obj + d_ptr);
   }else if(d_tag == DT_REL){
      d_ptr = (is_Elf32 ? swap32(dyn->d_un.d_ptr) : swap64(dyn->d_un.d_ptr));
      info->rel = (void*)(info->obj + d_ptr);
   }else if(d_tag == DT_RELA){
      d_ptr = (is_Elf32 ? swap32(dyn->d_un.d_ptr) : swap64(dyn->d_un.d_ptr));
      info->rela = (void*)(info->obj + d_ptr);
   }else if(d_tag == DT_PLTRELSZ){
      d_val = (is_Elf32 ? swap32(dyn->d_un.d_val) : swap64(dyn->d_un.d_val));
      info->pltrelsz = d_val;
   }else if(d_tag == DT_RELSZ){
      d_val = (is_Elf32 ? swap32(dyn->d_un.d_val) : swap64(dyn->d_un.d_val));
      info->relsz = d_val;
   }else if(d_tag == DT_RELASZ){
      d_val = (is_Elf32 ? swap32(dyn->d_un.d_val) : swap64(dyn->d_un.d_val));
      info->relasz = d_val;
   }else if(d_tag == DT_RELENT){
      d_val = (is_Elf32 ? swap32(dyn->d_un.d_val) : swap64(dyn->d_un.d_val));
      info->relent = d_val;
   }else if(d_tag == DT_RELAENT){
      d_val = (is_Elf32 ? swap32(dyn->d_un.d_val) : swap64(dyn->d_un.d_val));
      info->relaent = d_val;
   }else if(d_tag == DT_PLTREL){
      d_val = (is_Elf32 ? swap32(dyn->d_un.d_val) : swap64(dyn->d_un.d_val));
      info->pltrel = d_val;
   }
   if(d_tag != DT_NULL){
      ++dyn;
      goto read_tag;
   }
}
