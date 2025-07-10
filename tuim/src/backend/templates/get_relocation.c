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
#include "elf.h"
#include "tuim_backend.h"
/* ------------------------------------
   template for the teuim_get_relocation function
   * Part of Tuim Project.
   * Last modified: July 08, 2025.
------------------------------------ */

#if 1
   #define USE_ELF32_TEMPLATE
#endif

#define Swap_Sword  swap32
#define Swap_Word   swap32
#define Swap_Sxword swap64
#define Swap_Xword  swap64

#if defined(USE_ELF32_TEMPLATE)
   #define READ_DYNAMIC_TABLE read_dynamic_table32
   #define Elf(x) Elf32_##x
   #define Swap_Addr swap32
#elif defined(USE_ELF64_TEMPLATE)
   #define READ_DYNAMIC_TABLE read_dynamic_table64
   #define Elf(x) Elf64_##x
   #define Swap_Addr swap64
#endif

#define FILE_OFFSET(offset) (void*)(info->obj + (offset))

static void READ_DYNAMIC_TABLE(struct tuim_backend *info){
   const Elf(Dyn) *dyn = info->dyn;
   Elf(Addr) d_ptr;
   #if defined(USE_ELF32_TEMPLATE)
      Elf32_Word d_val;
      Elf32_Sword d_tag;
   #elif defined(USE_ELF64_TEMPLATE)
      Elf64_Xword d_val;
      Elf64_Xsword d_tag;
   #endif

   info->rel = NULL;
   info->rela = NULL;
   info->pltrel = DT_NULL;

   info->relent = info->relaent = SIZE_C(0);

   #if defined(USE_ELF32_TEMPLATE)
      read_tag: d_tag = Swap_Sword(dyn->d_tag);
   #elif defined(USE_ELF64_TEMPLATE)
      read_tag: d_tag = Swap_Sxword(dyn->d_tag);
   #endif
   if(d_tag == DT_JMPREL){
      d_ptr = Swap_Addr(dyn->d_un.d_ptr);
      info->jmprel = FILE_OFFSET(d_ptr);
   }else if(d_tag == DT_REL){
      d_ptr = Swap_Addr(dyn->d_un.d_ptr);
      info->rel = FILE_OFFSET(d_ptr);
   }else if(d_tag == DT_RELA){
      d_ptr = Swap_Addr(dyn->d_un.d_ptr);
      info->rela = FILE_OFFSET(d_ptr);
   }else
   #if defined(USE_ELF32_TEMPLATE)
      if(d_tag == DT_PLTRELSZ){
         d_val = Swap_Word(dyn->d_un.d_val);
         info->pltrelsz = d_val;
      }else if(d_tag == DT_RELSZ){
         d_val = Swap_Word(dyn->d_un.d_val);
         info->relsz = d_val;
      }else if(d_tag == DT_RELASZ){
         d_val = Swap_Word(dyn->d_un.d_val);
         info->relasz = d_val;
      }else if(d_tag == DT_RELENT){
         d_val = Swap_Word(dyn->d_un.d_val);
         info->relent = d_val;
      }else if(d_tag == DT_RELAENT){
         d_val = Swap_Word(dyn->d_un.d_val);
         info->relaent = d_val;
      }else if(d_tag == DT_PLTREL){
         d_val = Swap_Word(dyn->d_un.d_val);
         info->pltrel = d_val;
      }
   #elif defined(USE_ELF64_TEMPLATE)
      if(d_tag == DT_PLTRELSZ){
         d_val = Swap_Xword(dyn->d_un.d_val);
         info->pltrelsz = d_val;
      }else if(d_tag == DT_RELSZ){
         d_val = Swap_Xword(dyn->d_un.d_val);
         info->relsz = d_val;
      }else if(d_tag == DT_RELASZ){
         d_val = Swap_Xword(dyn->d_un.d_val);
         info->relasz = d_val;
      }else if(d_tag == DT_RELENT){
         d_val = Swap_Xword(dyn->d_un.d_val);
         info->relent = d_val;
      }else if(d_tag == DT_RELAENT){
         d_val = Swap_Xword(dyn->d_un.d_val);
         info->relaent = d_val;
      }else if(d_tag == DT_PLTREL){
         d_val = Swap_Xword(dyn->d_un.d_val);
         info->pltrel = d_val;
      }
   #endif
   if(d_tag != DT_NULL){
      ++dyn;
      goto read_tag;
   }

   if(info->relent == SIZE_C(0))
      info->relent = sizeof(Elf(Rel));
   if(info->relaent == SIZE_C(0))
      info->relaent = sizeof(Elf(Rela));
}

#undef READ_DYNAMIC_TABLE
#undef Elf
#undef Swap_Addr
