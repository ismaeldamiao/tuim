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
#include "elf.h"
#include "main.h"
/* ------------------------------------
   Processor dependent functions that the ELF interpreter needs.
   * Part of Tuim Project.
   * Last modified: February 24, 2025.
------------------------------------ */

static bool tuim_HaveCorrectAttributes(void *_ehdr, bool is_executable){
   Elf_Ehdr *ehdr = _ehdr;
   uint8_t *e_ident = ehdr->e_ident;

   return (
      (e_ident[EI_CLASS] == ELFCLASS64)
      && (e_ident[EI_DATA] == ELFDATA2LSB)
      && (e_ident[EI_OSABI] == ELFOSABI_NONE)
      && (
         ((ehdr->e_type == ET_EXEC) && is_executable)
         || ((ehdr->e_type == ET_DYN) && !is_executable)
      )
      && (e_ident[EI_VERSION] == EV_CURRENT)
      && (ehdr->e_version == EV_CURRENT)
      && (ehdr->e_machine == EM_X86_64)
   );
}

static void relocate(elf_s *elf){
   uint8_t
      *obj = elf->file.obj,
      *program = elf->image.program,
      *strtab = elf->dynsym.str;
   Elf_Addr start_vaddr = elf->image.start_vaddr;
   Elf_Ehdr *ehdr = (Elf_Ehdr*)obj;

   for(size_t i = SIZE_C(0); i < (size_t)(ehdr->e_shnum); ++i){
      uint8_t *reltab;
      size_t relnum, relsz;
      Elf_Shdr *shdr;
      Elf_Rel *rel;
      Elf_Rela *rela;
      Elf_Sym *symtab;

      shdr = Shdr_entry(i);
      if((shdr->sh_type != SHT_REL) && (shdr->sh_type != SHT_RELA)) continue;

      reltab = obj + shdr->sh_offset;
      symtab = (Elf_Sym*)(obj + Shdr_entry(shdr->sh_link)->sh_offset);

      relsz = ((shdr->sh_type == SHT_REL) ? sizeof(Elf_Rel) : sizeof(Elf_Rela));
      relnum = shdr->sh_size / relsz;

      for(size_t j = SIZE_C(0); j < relnum; ++j){
         rela = (Elf_Rela*)(reltab + j * relsz); // Caution!
         rel = (Elf_Rel*)rela;
/* ------------------------------------
AMD64 relocations
------------------------------------ */
   uint8_t *symbol;
   int64_t A;
   uint16_t st_shndx;
   uint64_t S, G, L, Z;
   uintptr_t P, B, GOT;
   Elf_Sym *sym;
   int r_type;
   size_t r_sym;

   S = (Elf_Addr)NULL;
   symbol = (uint8_t*)'\0';

   r_sym = ELF32_R_SYM(rel->r_info);
   r_type = ELF32_R_TYPE(rel->r_info);

   // Address of the symbol reference (place)
   P = (Elf_Addr)(program + (rela->r_offset - start_vaddr));

#if defined(__LP64__)
   A = rela->r_addend;
#endif // defined(__LP64__)

   // Base address of the symbol definition
   if(r_sym != UINT32_C(0)){

      sym = symtab + r_sym;
      symbol = strtab + sym->st_name;

      if(sym->st_shndx != STN_UNDEF){
         B = (Elf_Addr)(program - start_vaddr);
      }else{
         // The symbol is not defined in this ELF.
         for(elf_s **dep = elf->dep; *dep != NULL; ++dep){
            void *s;
            s = tuim_FindSymbol(symbol, *dep);
            if(s != NULL){
               B = (Elf_Addr)((*dep)->image.program - (*dep)->image.start_vaddr);
               sym = s;
               break;
            }
         }
      }
   }

   if(r_type == R_X86_64_RELATIVE){
#if defined(__ILP32__)
      A = ((shdr->sh_type != SHT_REL) ?
         (int64_t)(*((int32_t*)P)) : rela->r_addend
      );
#endif // defined(__ILP32__)
      /* RELATIVE relocations with undefined symbols use the addend
         as the address of symbols definition. */
      if(r_sym == UINT32_C(0))
         B =(Elf_Addr)(program - start_vaddr);
      *((unsigned long*)P) = (unsigned long)((uint64_t)B + A);
      continue;
   }
#if defined(__ILP32__)
   if(r_type == R_X86_64_RELATIVE64){
      A = ((shdr->sh_type != SHT_REL) ?
         (int64_t)(*((int64_t*)P)) : rela->r_addend
      );
      if(r_sym == UINT32_C(0))
         B = (Elf_Addr)(program - start_vaddr);
      *((uint64_t*)P) = (uint64_t)B + A;
      continue;
   }
#endif // defined(__ILP32__)

   /* word8 relocations */
   if(
      (r_type == R_X86_64_8) || (r_type == R_X86_64_PC8)
   ){

#if defined(__ILP32__)
      A = ((shdr->sh_type != SHT_REL) ?
         (int64_t)(*((int8_t*)P)) : rela->r_addend
      );
#endif // defined(__ILP32__)
   }
/* ------------------------------------
AMD64 relocations
------------------------------------ */
      }
   }
}

#include "tuim_FindSymbol.c"
