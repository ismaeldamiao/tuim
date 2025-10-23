/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 09, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

#if !defined(USE_ELF32_TEMPLATE) && !defined(USE_ELF64_TEMPLATE)
   #define USE_ELF32_TEMPLATE
#endif

#define Swap_Half  swap16
#define Swap_Word  swap32
#define Swap_Xword swap64

#if defined(USE_ELF32_TEMPLATE)
   #define Swap_Off  swap32
   #define Swap_Addr swap32
   #define Elf(x)    Elf32_##x
#elif defined(USE_ELF64_TEMPLATE)
   #define Swap_Off  swap64
   #define Swap_Addr swap64
   #define Elf(x)    Elf64_##x
#endif

#define FILE_OFFSET(offset) (void*)(obj + (offset))
#define LINK_ADDRESS(elf, v_addr) \
   ((elf)->program_image + ((v_addr) - (elf)->start_vaddr))

/*******************************************************************************
 * Function to get a dynamic symbol table  entry. For more information read
 * "System V Application Binary Interface", Chapter 5.
 *
 * The elf variable shall points to a valid initialized tuim_elf structure.
 * Otherwise the behavior is undefined.
 *
 * The symbol variable shall points to the desired symbol name.
 *
 * The function return the dynamic symbol table entry.
 *
 * NOTE: The current implementation support both ELF32 and ELF64 standard file
 * formats.
 ******************************************************************************/
static const void * Elf(sym)(const tuim_elf *elf, const Elf(Byte) *symbol){
   const Elf(Sym)  * const dynsym = elf->dynsym;
   const Elf(Byte) * const dynstr = elf->dynstr;

   const Elf(Sym)  * sym;
   size_t i, i_max;

   i_max = elf->sh_size / elf->sh_entsize;
   for(i = elf->sh_info; i < i_max; ++i){
      sym = (void*)((Elf(Byte)*)dynsym + i * elf->sh_entsize);
      if(strcmp((void*)symbol, (void*)(dynstr +Swap_Word(sym->st_name))) == 0){
         return sym;
      }
   }
   return NULL;
}

/*******************************************************************************
 * Function to get a symbol definition. For more information read
 * "System V Application Binary Interface", Chapter 5.
 *
 * The elf variable shall points to a valid initialized tuim_elf structure.
 * Otherwise the behavior is undefined.
 *
 * The symbol variable shall points to the desired symbol name.
 *
 * The function return address where the symbol is defined.
 *
 * NOTE: The current implementation support both ELF32 and ELF64 standard file
 * formats.
 ******************************************************************************/
static uintptr_t Elf(symbol)(const tuim_elf *elf, const Elf(Byte) *symbol){
   const Elf(Byte) * obj = elf->map;
   const Elf(Ehdr) * const ehdr = FILE_OFFSET(0x0);

   const Elf(Sym)  * sym;
   Elf(Section) st_shndx;
   Elf(Addr) address;

   if(symbol == NULL){
      address = Swap_Addr(ehdr->e_entry);
      address = LINK_ADDRESS(elf, address);
      return address;
   }

   sym = Elf(sym)(elf, symbol);
   if(sym != NULL){
      st_shndx = Swap_Half(sym->st_shndx);
      if(st_shndx == STN_UNDEF){
         return (uintptr_t)NULL;
      }else if(st_shndx == SHN_XINDEX){
         return (uintptr_t)NULL; /* FIXME: Find for it on SHT_SYMTAB_SHNDX */
      }else{
         address = Swap_Addr(sym->st_value);
         return LINK_ADDRESS(elf, address);
      }
   }
   return (uintptr_t)NULL;
}

#undef FILE_OFFSET
#undef LINK_ADDRESS

#undef Swap_Half
#undef Swap_Word
#undef Swap_Xword
#undef Elf
