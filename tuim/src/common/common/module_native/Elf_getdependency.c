/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 10, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

#if !defined(USE_ELF32_TEMPLATE) && !defined(USE_ELF64_TEMPLATE)
   #define USE_ELF32_TEMPLATE
#endif

#define Swap_Word  swap32
#define Swap_Xword swap64

#define Swap_Sword  swap32
#define Swap_Sxword swap64

#if defined(USE_ELF32_TEMPLATE)
   #define Elf(x) Elf32_##x
#elif defined(USE_ELF64_TEMPLATE)
   #define Elf(x) Elf64_##x
#endif

/*******************************************************************************
 * Function to parse a ELF file and return the next DT_NEEDED entry from
 * dynamic table. For more information read "System V Application Binary
 * Interface", Chapter 5.
 *
 * The elf variable shall points to a valid initialized tuim_elf structure.
 * Otherwise the behavior is undefined.
 *
 * The function return a pointer to the entry on the dynamic string table
 * holding the name of the dependency, if no dependency are found the function
 * return NULL.
 *
 * NOTE: The current implementation support both ELF32 and ELF64 standard file
 * formats.
 ******************************************************************************/
static const Elf(Byte) *Elf(getdependency)(tuim_elf *elf){
   const Elf(Byte) *dynstr = elf->dynstr;
   const Elf(Dyn) *dyn;
   #if defined(USE_ELF32_TEMPLATE)
      Elf32_Word d_val;
      Elf32_Sword d_tag;
   #elif defined(USE_ELF64_TEMPLATE)
      Elf64_Xword d_val;
      Elf64_Sxword d_tag;
   #endif

   if(elf->last.needed == NULL){
      if(elf->dyn == NULL) return NULL;
      elf->last.needed = elf->dyn;
   }

   dyn = elf->last.needed;

   #if defined(USE_ELF32_TEMPLATE)
      read_tag: d_tag = Swap_Sword(dyn->d_tag);
   #elif defined(USE_ELF64_TEMPLATE)
      read_tag: d_tag = Swap_Sxword(dyn->d_tag);
   #endif

   if(d_tag == DT_NULL){ elf->last.needed = NULL; return NULL; }
   if(d_tag != DT_NEEDED){ ++dyn; goto read_tag; }

   #if defined(USE_ELF32_TEMPLATE)
      d_val = Swap_Word (dyn->d_un.d_val);
   #elif defined(USE_ELF64_TEMPLATE)
      d_val = Swap_Xword(dyn->d_un.d_val);
   #endif

   elf->last.needed = dyn + 1;
   return dynstr + d_val;
}

#undef Swap_Word
#undef Swap_Xword
#undef Swap_Sword
#undef Swap_Sxword
#undef Elf
