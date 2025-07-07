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

#define addressof(x) ((uint32_t)(x))

#define DEBUG 0

#if DEBUG
#define DBG(str, ...) printf(str, __VA_ARGS__)
int printf(const char *, ...);
int puts(const char *);
#else
#define DBG(str, ...)
#endif

#ifndef swap32
   #define swap32(x) (x)
#endif

#ifndef swap16
   #define swap16(x) (x)
#endif

const uint8_t *
tuim_relocate(const tuim_ctx *ctx, void *ptr, const void *_rel, const void *_dep){
   struct tuim_backend *info = ptr;
   struct tuim_backend * const * dep = _dep;
   const Elf32_Rel *rel = _rel;
   const Elf32_Rela *rela = _rel;
   const Elf32_Sym *dynsym = info->dynsym;

   const uint8_t *symbol;
   const Elf32_Sym *sym;
   uint8_t *P, *B, *S;
   int32_t A;
   int r_type;
   size_t r_sym;

   (void)ctx;

   S = NULL;
   symbol = (uint8_t*)"\0";

   r_sym = ELF32_R_SYM(swap32(rel->r_info));
   r_type = ELF32_R_TYPE(swap32(rel->r_info));

   // Relocation types with no action
   if((r_type == R_ARM_TLS_DESC) || (r_type == R_ARM_COPY)) return NULL;
   // Support to outdated linkers
   if(r_type == R_ARM_ABS32) r_type = R_ARM_GLOB_DAT;

   // Address of the symbol reference (place)
   P = (void*)(
      (Elf32_Byte*)(info->program_image) +
      (swap32(rel->r_offset) - info->start_vaddr)
   );

   /* From _ELF for the Arm Architecture_:
      > If the place is subject to a data-type relocation,
      > the initial value in the place is sign-extended to 32
      > bits.
   */
   // The addend
   A = ((info->is_rel) ? swap32(*((int32_t*)P)) : swap32(rela->r_addend));

   // Base address of the symbol definition
   if(r_sym != UINT32_C(0)){

      sym = dynsym + r_sym;
      symbol = info->dynstr + swap32(sym->st_name);

      if(sym->st_shndx != STN_UNDEF){
         B = (Elf32_Byte*)(info->program_image) - info->start_vaddr;
      }else{
         // The symbol is not defined in this ELF.
         void *s;
         for(dep = _dep; *dep != NULL; ++dep){
            s = (void*)tuim_get_symbol(ctx, *dep, symbol);
            // printf("Finding on %s\n", (*dep)->file.name);
            if(s != NULL){
               B = (Elf32_Byte*)((*dep)->program_image) - (*dep)->start_vaddr;
               S = s;
               break;
            }
         }
         if(s == NULL){
            return symbol;
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
      return NULL;
   }

   // Address of the symbol definition
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
      if(ELF32_ST_TYPE(sym->st_info) == STT_FUNC)
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
      if(info->is_rel) A = INT32_C(0);
      if(ELF32_ST_TYPE(sym->st_info) == STT_FUNC)
         T = addressof(S) & UINT32_C(0x00000001);
      else
         T = UINT32_C(0x00000000);
      *((uint32_t*)P) = addressof(S + A) | T;
      DBG(
         "   R_ARM_JUMP_SLOT(%s): %p <- 0x%08x.\n",
         string(symbol), P, *((uint32_t*)P)
      );
   }

   return NULL;
}
