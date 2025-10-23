/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 10, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

#if !defined(USE_ELF32_TEMPLATE) && !defined(USE_ELF64_TEMPLATE)
   #define USE_ELF32_TEMPLATE
#endif

#define Swap_Sword  swap32
#define Swap_Word   swap32
#define Swap_Sxword swap64
#define Swap_Xword  swap64

#if defined(USE_ELF32_TEMPLATE)
   #define Swap_Addr swap32
   #define Elf(x) Elf32_##x
#elif defined(USE_ELF64_TEMPLATE)
   #define Swap_Addr swap64
   #define Elf(x) Elf64_##x
#endif

#define FILE_OFFSET(offset) (void*)(obj + (offset))

/*******************************************************************************
 * Function to parse a ELF file and return the next DT_JMPREL, DT_REL or
 * DT_RELA entry from dynamic table. For more information read "System V
 * Application Binary Interface", Chapter 5.
 *
 * The elf variable shall points to a valid initialized tuim_elf structure.
 * Otherwise the behavior is undefined.
 *
 * The function return a pointer to the relocation table entry and set
 * elf->is_rel to true when the entry have no explicity addend.
 *
 * NOTE: The current implementation support both ELF32 and ELF64 standard file
 * formats.
 ******************************************************************************/
static const void* Elf(getrelocation)(tuim_elf *elf){
   const uint8_t *obj = elf->map;
   const Elf(Dyn) *dyn = elf->dyn;
   Elf(Addr) d_ptr;
   #if defined(USE_ELF32_TEMPLATE)
      Elf32_Word d_val;
      Elf32_Sword d_tag;
   #elif defined(USE_ELF64_TEMPLATE)
      Elf64_Xword d_val;
      Elf64_Sxword d_tag;
   #endif
   void *next;

   if(elf->last.rel == NULL){
      if(elf->dyn == NULL) return NULL;
      goto not_initialized;
   }

   if(elf->jmprel != NULL){
      next = (uint8_t*)(elf->last.rel) + (
         (elf->pltrel == DT_REL) ? elf->relent : elf->relaent);
      if(next != (uint8_t*)(elf->jmprel) + elf->pltrelsz){
         elf->last.rel = next;
      }else{
         elf->jmprel = NULL;

         elf->is_rel = (elf->rel != NULL);
         elf->is_rela = (!(elf->is_rel) && (elf->rela != NULL));
         elf->last.rel = ((elf->rel != NULL) ? elf->rel : elf->rela);
      }
   }else if(elf->rel != NULL){
      next = (uint8_t*)(elf->last.rel) + elf->relent;
      if(next != (uint8_t*)(elf->rel) + elf->relsz){
         elf->last.rel = next;
      }else{
         elf->rel = NULL;

         elf->is_rel = false;
         elf->is_rela = (elf->rela != NULL);
         elf->last.rel = elf->rela;
      }
   }else if(elf->rela != NULL){
      next = (uint8_t*)(elf->last.rel) + elf->relaent;
      if(next != (uint8_t*)(elf->rela) + elf->relasz){
         elf->last.rel = next;
      }else{
         elf->rela = NULL;

         elf->is_rela = false;
         elf->last.rel = NULL;
      }
   }

   return elf->last.rel;

   not_initialized:

   elf->rel = NULL;
   elf->rela = NULL;
   elf->pltrel = DT_NULL;

   elf->relent = elf->relaent = SIZE_C(0);

   #if defined(USE_ELF32_TEMPLATE)
      read_tag: d_tag = Swap_Sword(dyn->d_tag);
   #elif defined(USE_ELF64_TEMPLATE)
      read_tag: d_tag = Swap_Sxword(dyn->d_tag);
   #endif
   if(d_tag == DT_JMPREL){
      d_ptr = Swap_Addr(dyn->d_un.d_ptr);
      elf->jmprel = FILE_OFFSET(d_ptr);
   }else if(d_tag == DT_REL){
      d_ptr = Swap_Addr(dyn->d_un.d_ptr);
      elf->rel = FILE_OFFSET(d_ptr);
   }else if(d_tag == DT_RELA){
      d_ptr = Swap_Addr(dyn->d_un.d_ptr);
      elf->rela = FILE_OFFSET(d_ptr);
   }else
   #if defined(USE_ELF32_TEMPLATE)
      if(d_tag == DT_PLTRELSZ){
         d_val = Swap_Word(dyn->d_un.d_val);
         elf->pltrelsz = d_val;
      }else if(d_tag == DT_RELSZ){
         d_val = Swap_Word(dyn->d_un.d_val);
         elf->relsz = d_val;
      }else if(d_tag == DT_RELASZ){
         d_val = Swap_Word(dyn->d_un.d_val);
         elf->relasz = d_val;
      }else if(d_tag == DT_RELENT){
         d_val = Swap_Word(dyn->d_un.d_val);
         elf->relent = d_val;
      }else if(d_tag == DT_RELAENT){
         d_val = Swap_Word(dyn->d_un.d_val);
         elf->relaent = d_val;
      }else if(d_tag == DT_PLTREL){
         d_val = Swap_Word(dyn->d_un.d_val);
         elf->pltrel = d_val;
      }
   #elif defined(USE_ELF64_TEMPLATE)
      if(d_tag == DT_PLTRELSZ){
         d_val = Swap_Xword(dyn->d_un.d_val);
         elf->pltrelsz = d_val;
      }else if(d_tag == DT_RELSZ){
         d_val = Swap_Xword(dyn->d_un.d_val);
         elf->relsz = d_val;
      }else if(d_tag == DT_RELASZ){
         d_val = Swap_Xword(dyn->d_un.d_val);
         elf->relasz = d_val;
      }else if(d_tag == DT_RELENT){
         d_val = Swap_Xword(dyn->d_un.d_val);
         elf->relent = d_val;
      }else if(d_tag == DT_RELAENT){
         d_val = Swap_Xword(dyn->d_un.d_val);
         elf->relaent = d_val;
      }else if(d_tag == DT_PLTREL){
         d_val = Swap_Xword(dyn->d_un.d_val);
         elf->pltrel = d_val;
      }
   #endif
   if(d_tag != DT_NULL){
      ++dyn;
      goto read_tag;
   }

   if(elf->relent == SIZE_C(0))
      elf->relent = sizeof(Elf(Rel));
   if(elf->relaent == SIZE_C(0))
      elf->relaent = sizeof(Elf(Rela));

   if(elf->pltrel != DT_NULL){
      elf->is_rel =  (elf->pltrel == DT_REL);
      elf->is_rela =  (elf->pltrel == DT_RELA);
      elf->last.rel = elf->jmprel;
   }else{
      elf->is_rel = (elf->rel != NULL);
      elf->is_rela = (!(elf->is_rel) && (elf->rela != NULL));
      elf->last.rel = ((elf->rel != NULL) ? elf->rel : elf->rela);
   }
   return elf->last.rel;
}

#undef Swap_Sword
#undef Swap_Word
#undef Swap_Sxword
#undef Swap_Xword
#undef Swap_Addr
#undef FILE_OFFSET
#undef Elf
