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
#include "tuim_backend.h"
/* ------------------------------------
   Relocate EM_ARM files.
   * Reference document:
     ELF for the Arm (R) Architecture
     https://github.com/ARM-software/abi-aa/blob/main/aaelf32/aaelf32.rst
   * Date of Issue: 07th April 2025
   * Part of Tuim Project.
   * Last modified: July 10, 2025.
------------------------------------ */

#define Swap_Half  swap16
#define Swap_Sword swap32
#define Swap_Word  swap32
#define Swap_Addr  swap32

#define LINK_ADDRESS(info, v_addr) \
   ((info)->program_image + ((v_addr) - (info)->start_vaddr))

const Elf32_Byte * tuim_relocate(
   const  tuim_ctx *ctx,
   void *ptr,
   const void *_rel, const void *_dep){

   struct tuim_backend * const info  = ptr;
   struct tuim_backend * const * dep = _dep;
   const Elf32_Rel  * const rel    = _rel;
   const Elf32_Rela * const rela   = _rel;
   const Elf32_Sym  * const dynsym = info->dynsym;
   const Elf32_Byte ascii_null = 0;

   const Elf32_Sym  * sym;

   const Elf32_Byte * symbol;
   Elf32_Half st_shndx;
   Elf32_Sword A;
   Elf32_Word r_info, result, st_size;
   Elf32_Addr r_offset, P, T, S;

   int r_type;
   size_t r_sym;

   /* --- initialize relocation variables --- */

   symbol = &ascii_null;

   r_info = Swap_Word(rel->r_info);
   r_sym  = ELF32_R_SYM (r_info);
   r_type = ELF32_R_TYPE(r_info);

   /* From _ELF for the Arm Architecture_:
      > P is the address of the place being relocated
      > (derived from r_offset). */
   r_offset = Swap_Addr(rel->r_offset);
   P = LINK_ADDRESS(info, r_offset);

   /* From _ELF for the Arm Architecture_:
      > A is the addend for the relocation.

      > With the exception of R_ARM_COPY all dynamic relocations
      > require that the place being relocated is a
      > word-aligned 32-bit object.

      > If the place is subject to a data-type relocation,
      > the initial value in the place is sign-extended to 32
      > bits. */
   if(r_type == R_ARM_COPY) goto have_addend;
   if(info->is_rela){
      A = Swap_Sword(rela->r_addend);
   }else{
      if(r_type == R_ARM_JUMP_SLOT){
         /* From _ELF for the Arm Architecture_:
            > In a REL form of this relocation the addend, A, is always 0. */
         A = INT32_C(0);
      }else{
         tuim_load4(ctx, &A, P, 1);
      }
   }

   have_addend:

   /* From _ELF for the Arm Architecture_:
      > S (when used on its own) is the address of the symbol. */
   if(r_sym == 0){
      /* there is no symbol reference */
      S = 0;
      goto relocate;
   }

   sym = dynsym + r_sym;
   symbol = info->dynstr + Swap_Word(sym->st_name);

   if(sym->st_shndx != STN_UNDEF){
      /* the symbol is defined here */
      S = LINK_ADDRESS(info, Swap_Word(sym->st_value));
      // TODO: Handle st_shndx == SHN_XINDEX
      goto have_symbol;
   }

   /* the symbol is defined in another place */
   for(dep = _dep; *dep != NULL; ++dep){
      sym = tuim_get_sym(*dep, symbol);
      if(sym == NULL) continue;
      st_shndx = Swap_Half(sym->st_shndx);
      if(st_shndx != STN_UNDEF) break;
   }
   if((sym == NULL) || (st_shndx == STN_UNDEF)) return symbol;

   S = LINK_ADDRESS(*dep, Swap_Word(sym->st_value));
   // TODO: Handle st_shndx == SHN_XINDEX

   have_symbol:

   /* From _ELF for the Arm Architecture_:
      > T is 1 if the target symbol S has type STT_FUNC and 
      > the symbol addresses a Thumb instruction; it is
      > 0 otherwise. */
   if(ELF32_ST_TYPE(sym->st_info) == STT_FUNC)
      T = S & UINT32_C(0x00000001);
   else
      T = UINT32_C(0x00000000);

   /* --- relocate --- */
   relocate:

   /* Support to outdated linkers */
   if(r_type == R_ARM_ABS32) r_type = R_ARM_GLOB_DAT;

   switch(r_type){
      case R_ARM_BREL_ADJ:
         /* ChangeIn[B(S)] + A */
         break;
      case R_ARM_TLS_DESC:
         /* */
         break;
      case R_ARM_TLS_DTPMOD32:
         /* Module[S] */
         break;
      case R_ARM_TLS_DTPOFF32:
         /* S + A – TLS */
         break;
      case R_ARM_TLS_TPOFF32:
         /* S + A – tp */
         break;
      case R_ARM_COPY:
         /* */
         st_size = Swap_Word(sym->st_size);
         tuim_memcpy(ctx, P, S, st_size);
         DBG("   R_ARM_COPY(%s): 0x%08x <- 0x%08x.\n", string(symbol), P, S);
         break;
      case R_ARM_GLOB_DAT:
         /* (S + A) | T */
         result = (S + A) | T;
         tuim_store4(ctx, P, &result, 1);
         DBG("   R_ARM_GLOB_DAT(%s): 0x%08x <- 0x%08x.\n",
         string(symbol), P, result);
         break;
      case R_ARM_JUMP_SLOT:
         /* (S + A) | T */
         result = (S + A) | T;
         tuim_store4(ctx, P, &result, 1);
         DBG("   R_ARM_JUMP_SLOT(%s): 0x%08x <- 0x%08x.\n",
         string(symbol), P, result);
         break;
      case R_ARM_RELATIVE:
         /* B(S) + A [Note: see Dynamic relocations] */
         result = ((S == 0) ? LINK_ADDRESS(info, A) : LINK_ADDRESS(*dep, A));
         tuim_store4(ctx, P, &result, 1);
         DBG("   R_ARM_RELATIVE(%s): 0x%08x <- 0x%08x.\n",
         string(symbol), P, result);
         break;
      case R_ARM_IRELATIVE:
         /* Reserved for future functionality */
         break;
      default:
   }

   return NULL;
}
