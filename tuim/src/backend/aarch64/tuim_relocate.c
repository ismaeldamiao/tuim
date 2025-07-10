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
     ELF for the Arm (R) 64-bit Architecture (AArch64)
     https://github.com/ARM-software/abi-aa/blob/main/aaelf64/aaelf64.rst
   * Date of Issue: 07th April 2025
   * Part of Tuim Project.
   * Last modified: July 10, 2025.
------------------------------------ */

#define Swap_Half   swap16
#define Swap_Sword  swap32
#define Swap_Word   swap32
#define Swap_Sxword swap64
#define Swap_Xword  swap64
#define Swap_Addr   swap64

#define LINK_ADDRESS(info, v_addr) \
   ((info)->program_image + ((v_addr) - (info)->start_vaddr))

const Elf32_Byte * tuim_relocate(
   const  tuim_ctx *ctx,
   void *ptr,
   const void *_rel, const void *_dep){

   struct tuim_backend * const info  = ptr;
   struct tuim_backend * const * dep = _dep;
   const Elf64_Rel  * const rel    = _rel;
   const Elf64_Rela * const rela   = _rel;
   const Elf64_Sym  * const dynsym = info->dynsym;
   const Elf64_Byte ascii_null = 0;

   const Elf64_Sym  * sym;

   const Elf64_Byte * symbol;
   Elf64_Half st_shndx;
   Elf64_Sword A;
   Elf64_Xword r_info, result, st_size;
   Elf64_Addr r_offset, P, B, S, Delta;

   int r_type;
   size_t r_sym;

   /* --- initialize relocation variables --- */

   symbol = &ascii_null;

   r_info = Swap_Xword(rel->r_info);
   r_sym  = ELF64_R_SYM (r_info);
   r_type = ELF64_R_TYPE(r_info);

   /* From _ELF for the Arm Architecture_:
      > P is the address of the place being relocated
      > (derived from r_offset). */
   r_offset = Swap_Addr(rel->r_offset);
   P = LINK_ADDRESS(info, r_offset);

   /* From _ELF for the Arm Architecture_:
      > A is the addend for the relocation.

      > With the exception of R_<CLS>_COPY all dynamic relocations require
      > that the place being relocated is an 8-byte aligned 64-bit data
      > location in ELF64 or a 4-byte aligned 32-bit data location in ELF32.

      > * If the relocation relocates data (Static Data relocations)
      >   the initial value in the place is sign-extended to 64 bits.
      > * If the relocation relocates an instruction the immediate field
      >   of the instruction is extracted, scaled as required by the
      >   instruction field encoding, and sign-extended to 64 bits. */
   if(r_type == R_AARCH64_COPY) goto have_addend;
   if(info->is_rela){
      A = Swap_Sxword(rela->r_addend);
   }else{
      if(r_type == R_AARCH64_JUMP_SLOT){
         /* From _ELF for the Arm Architecture_:
            > The addend A of such a REL-type relocation shall be zero. */
         A = INT64_C(0);
      }else{
         tuim_load8(ctx, &A, P, 1);
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
      goto relocate;
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

   /* --- relocate --- */
   relocate:

   switch(r_type){
      case(R_AARCH64_ABS64):
         /* S + A */
         break;
      case(R_AARCH64_AUTH_ABS64):
         /* SIGN(S + A, SCHEMA(*P)) */
         break;
      case(R_AARCH64_COPY):
         /*  */
         st_size = Swap_Word(sym->st_size);
         tuim_memcpy(ctx, P, S, st_size);
         break;
      case(R_AARCH64_GLOB_DAT):
         /* S + A */
      case(R_AARCH64_JUMP_SLOT):
         /* S + A */
         result = S + A;
         tuim_store8(ctx, P, &result, 1);
         break;
      case(R_AARCH64_RELATIVE):
         /* Delta(S) + A */
         result = ((S == 0) ? LINK_ADDRESS(info, A) : LINK_ADDRESS(*dep, A));
         tuim_store8(ctx, P, &result, 1);
         break;
      case(R_AARCH64_TLS_IMPDEF1):
         /* DTPREL(S+A) */
         break;
      case(R_AARCH64_TLS_IMPDEF2):
         /* LDM(S) */
         break;
      case(R_AARCH64_TLS_TPREL):
         /* TPREL(S+A) */
         break;
      case(R_AARCH64_TLSDESC):
         /* TLSDESC(S+A) */
         break;
      case(R_AARCH64_IRELATIVE):
         /* Indirect(Delta(S) + A) */
         break;
      case(R_AARCH64_AUTH_RELATIVE):
         /* SIGN(DELTA(S) + A, SCHEMA(*P)) */
         break;
      case(R_AARCH64_AUTH_GLOB_DAT):
         /* SIGN((S + A), SCHEMA(*P)) */
         break;
      case(R_AARCH64_AUTH_TLSDESC):
         /* SIGN(TLSDESC(S + A), SCHEMA(*P)) */
         break;
      case(R_AARCH64_AUTH_IRELATIVE):
         /* SIGN(Indirect(S + A), SCHEMA(*P)) */
         break;
      default:
   }

   return NULL;
}
