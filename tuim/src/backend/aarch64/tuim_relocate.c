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
   This function relocate the memory image of the program.
   * Part of Tuim Project.
   * Last modified: July 07, 2025.
------------------------------------ */

#include "../elf.h"

#include "../../api/tuim_ctx.h"
#include "../tuim_backend.h"


#define DEBUG 0
#if DEBUG
   #define DBG(str, ...) printf(str, __VA_ARGS__)
   #define string(x) ((char*)(x))
   int printf(const char *, ...);
   int puts(const char *);
#else
   #define DBG(str, ...)
#endif


const uint8_t *
tuim_relocate(const tuim_ctx *ctx, void *ptr, const void *_rel, const void *_dep){
   struct tuim_backend *info = ptr;
   struct tuim_backend * const * dep = _dep;
   const Elf64_Rel *rel = _rel;
   const Elf64_Rela *rela = _rel;
   const Elf64_Sym *dynsym = info->dynsym;

   const uint8_t *symbol;
   const Elf64_Sym *sym;
   Elf64_Addr P, B, S, Delta;
   Elf64_Sword A;
   int r_type;
   size_t r_sym;

   /* Dynamic relocations
      R_AARCH64_ABS64
      R_AARCH64_AUTH_ABS64
      R_AARCH64_COPY
      R_AARCH64_GLOB_DAT
      R_AARCH64_JUMP_SLOT
      R_AARCH64_RELATIVE
      R_AARCH64_TLS_IMPDEF1
      R_AARCH64_TLS_IMPDEF2
      R_AARCH64_TLS_DTPREL
      R_AARCH64_TLS_DTPMOD
      R_AARCH64_TLS_TPREL 
      R_AARCH64_TLSDESC
      R_AARCH64_IRELATIVE
      R_AARCH64_AUTH_RELATIVE */

   S = tuim_nullptr;
   symbol = (uint8_t*)"\0";

   r_sym = ELF64_R_SYM(swap64(rel->r_info));
   r_type = ELF64_R_TYPE(swap64(rel->r_info));

   // Address of the symbol reference (place)
   P = info->program_image + (swap64(rel->r_offset) - info->start_vaddr);

   /* From _ELF for the Arm Architecture_:
      > If the place is subject to a data-type relocation,
      > the initial value in the place is sign-extended to 32
      > bits.
   */

   // The addend
   if(info->is_rel){
      tuim_load8(ctx, &A, P, 1);
   }else{
      A = swap64(rela->r_addend);
   }
   // Base address of the symbol definition
   if(r_sym != UINT64_C(0)){

      sym = dynsym + r_sym;
      symbol = info->dynstr + swap32(sym->st_name);

      if(sym->st_shndx != STN_UNDEF){
         B = info->program_image - info->start_vaddr;
         S = B + swap64(sym->st_value);
         Delta = info->program_image - info->start_vaddr;
      }else{
         // The symbol is not defined in this ELF.
         for(dep = _dep; *dep != NULL; ++dep){
            S = tuim_get_symbol(ctx, *dep, symbol);
            if(S != tuim_nullptr){
               B = (*dep)->program_image - (*dep)->start_vaddr;
               break;
            }
         }
         if(S == tuim_nullptr){
            return symbol;
         }
      }
   }

   // Relocation types that do not need symbol value
   if(r_type == R_AARCH64_RELATIVE){
      Elf64_Xword tmp;
      /* RELATIVE relocations with undefined symbols use the addend
         as the address of symbols definition. */
      tmp = Delta + A;
      tuim_store8(ctx, P, &tmp, 1);
      DBG(
         "   R_ARM_RELATIVE(%s): %p <- 0x%08x.\n",
         string(symbol), P, *((uint32_t*)P)
      );
      return NULL;
   }

   if(r_type == R_AARCH64_GLOB_DAT){
      Elf64_Xword tmp;
      tmp = S + A;
      tuim_store8(ctx, P, &tmp, 1);
   }else if(r_type == R_AARCH64_JUMP_SLOT){
      Elf64_Xword tmp;
      tmp = S + A;
      tuim_store8(ctx, P, &tmp, 1);
   }

   return NULL;
}
