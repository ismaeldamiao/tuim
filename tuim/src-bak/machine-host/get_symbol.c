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
#include <string.h>
#include "elf.h"
#include "../tuim_impl.h"
/* ------------------------------------
   This function find by a symbol definition on dynamic symbol table.
   * Part of Tuim Project.
   * Last modified: July 15, 2025.
------------------------------------ */

#if 0
   #define USE_ELF32_TEMPLATE
#endif

#define Swap_Half  swap16
#define Swap_Word  swap32
#define Swap_Xword swap64

#if defined(USE_ELF32_TEMPLATE)
   #define GET_SYMBOL  get_symbol32
   #define GET_SYM     get_sym32

   #define Elf(x)    Elf32_##x
   #define Swap_Off  swap32
   #define Swap_Addr swap32
#elif defined(USE_ELF64_TEMPLATE)
   #define GET_SYMBOL  get_symbol64
   #define GET_SYM     get_sym64

   #define Elf(x)    Elf64_##x
   #define Swap_Off  swap64
   #define Swap_Addr swap64
#endif

#define FILE_OFFSET(offset) (void*)(obj + (offset))
#define LINK_ADDRESS(v_addr) \
   (elf->program_image + ((v_addr) - elf->start_vaddr))

extern const uint64_t tuim_target_nullptr;

static const void * GET_SYM(const tuim_elf *elf, const Elf(Byte) *symbol){
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

static uint64_t GET_SYMBOL(const tuim_elf *elf, const Elf(Byte) *symbol){
   const Elf(Byte) * obj = elf->file.obj;
   const Elf(Ehdr) * const ehdr = FILE_OFFSET(0x0);

   const Elf(Sym)  * sym;
   Elf(Section) st_shndx;
   Elf(Addr) address;

   if(symbol == NULL){
      address = Swap_Addr(ehdr->e_entry);
      address = LINK_ADDRESS(address);
      return address;
   }

   sym = GET_SYM(elf, symbol);
   if(sym != NULL){
      st_shndx = Swap_Half(sym->st_shndx);
      if(st_shndx == STN_UNDEF){
         return tuim_target_nullptr;
      }else if(st_shndx == SHN_XINDEX){
         return tuim_target_nullptr; /* FIXME: Find for it on SHT_SYMTAB_SHNDX */
      }else{
         address = Swap_Addr(sym->st_value);
         return LINK_ADDRESS(address);
      }
   }
   return tuim_target_nullptr;
}


#undef GET_SYMBOL
#undef GET_SYM
#undef Elf
#undef Swap_Off
#undef Swap_Addr
