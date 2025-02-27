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
   * Last modified: February 21, 2025.
------------------------------------ */

static bool tuim_HaveCorrectAttributes(void *_ehdr, bool is_executable){
   Elf_Ehdr *ehdr = _ehdr;
   uint8_t *e_ident = ehdr->e_ident;

   return (
      (e_ident[EI_CLASS] == ELFCLASS32)
#if defined(__ARM_BIG_ENDIAN)
      && (e_ident[EI_CLASS] == ELFDATA2MSB)
#else
      && (e_ident[EI_DATA] == ELFDATA2LSB)
#endif
      && (e_ident[EI_OSABI] == ELFOSABI_NONE)
      && (
         ((ehdr->e_type == ET_EXEC) && is_executable)
         || ((ehdr->e_type == ET_DYN) && !is_executable)
      )
      && (e_ident[EI_VERSION] == EV_CURRENT)
      && (ehdr->e_version == EV_CURRENT)
      && (ehdr->e_machine == EM_ARM)
      && (EF_ARM_EABI_VERSION(ehdr->e_flags) == EF_ARM_EABI_VER5)
#if !(__ARM_FP & 0x0C)
      /* If hardware floating point instructions are not present, then never
         load code conform to the software floating-point procedure-call
         standard. */
      && !(ehdr->e_flags & EF_ARM_ABI_FLOAT_HARD)
#endif
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
AARCH32 relocations
------------------------------------ */
   uint8_t *symbol, *P, *B, *S;
   Elf32_Sym *sym;
   int32_t A;
   int r_type;
   size_t r_sym;

   S = NULL;
   symbol = (uint8_t*)'\0';

   r_sym = ELF32_R_SYM(rel->r_info);
   r_type = ELF32_R_TYPE(rel->r_info);

   // Relocation types with no action
   if((r_type == R_ARM_TLS_DESC) || (r_type == R_ARM_COPY)) continue;
   // Support to outdated linkers
   if(r_type == R_ARM_ABS32) r_type = R_ARM_GLOB_DAT;

   // Address of the symbol reference (place)
   P = program + (rel->r_offset - start_vaddr);

   /* From _ELF for the Arm Architecture_:
      > If the place is subject to a data-type relocation,
      > the initial value in the place is sign-extended to 32
      > bits.
   */
   // The addend
   A = ((shdr->sh_type == SHT_REL) ? *((int32_t*)P) : rela->r_addend);

   // Base address of the symbol definition
   if(r_sym != UINT32_C(0)){

      sym = symtab + r_sym;
      symbol = strtab + sym->st_name;

      if(sym->st_shndx != STN_UNDEF){
         B = program - start_vaddr;
      }else{
         // The symbol is not defined in this ELF.
         for(elf_s **dep = elf->dep; *dep != NULL; ++dep){
            void *s;
            s = tuim_FindSymbol(symbol, *dep);
            if(s != NULL){
               B = (*dep)->image.program - (*dep)->image.start_vaddr;
               sym = s;
               break;
            }
         }
      }
   }

   // Relocation types that do not need symbol value
   if(r_type == R_ARM_RELATIVE){
      /* RELATIVE relocations with undefined symbols use the addend
         as the address of symbols definition. */
      *((uint32_t*)P) = *((uint32_t*)(B + A));
      DBG(
         "   R_ARM_RELATIVE(%s): %p <- 0x%08x.\n",
         string(symbol), P, *((uint32_t*)P)
      );
      continue;
   }

   // Address of the symbol definition
   // Non-NULL only if is a system call
   if((void*)S == NULL) S = B + sym->st_value;

   if(r_type == R_ARM_BREL_ADJ){
      // TODO:
      //*((uint32_t*)P) = /* ChangeIn[B(S)] + */ A;
   }else if(r_type == R_ARM_TLS_DTPMOD32){
      // TODO:
      //*((uint32_t*)P) = /*Module[*/ S /*]*/;
   }else if(r_type == R_ARM_TLS_DTPOFF32){
      // TODO:
      //*((uint32_t*)P) = S + A /* - TLS */;
   }else if(r_type == R_ARM_TLS_TPOFF32){
      // TODO:
      //*((uint32_t*)P) = S + A /* - tp */;
   }else if(r_type == R_ARM_GLOB_DAT){
      uint32_t T;
      if(ELF_ST_TYPE(sym->st_info) == STT_FUNC)
         T = addressof(S) & UINT32_C(0x00000001);
      else
         T = UINT32_C(0x00000000);

      *((uint32_t*)P) = addressof(S + A) | T;
      DBG(
         "   R_ARM_GLOB_DAT(%s): %p <- 0x%08x.\n",
         string(symbol), P, *((uint32_t*)P)
      );
   }else if(r_type == R_ARM_JUMP_SLOT){
      uint32_t T;
      if(shdr->sh_type == SHT_REL) A = INT32_C(0);
      if(ELF_ST_TYPE(sym->st_info) == STT_FUNC)
         T = addressof(S) & UINT32_C(0x00000001);
      else
         T = UINT32_C(0x00000000);
      *((uint32_t*)P) = addressof(S + A) | T;
      DBG(
         "   R_ARM_JUMP_SLOT(%s): %p <- 0x%08x.\n",
         string(symbol), P, *((uint32_t*)P)
      );
   }
/* ------------------------------------
AARCH32 relocations
------------------------------------ */
      }
   }
}

#include "tuim_FindSymbol.c"
