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
   Relocate EM_X86_64 files.
   * Reference document:
     System V Application Binary Interface
     AMD64 Architecture Processor Supplement
     (With LP64 and ILP32 Programming Models)
     Version 1.0
     https://gitlab.com/x86-psABIs/x86-64-ABI
   * Part of Tuim Project.
   * Last modified: July 10, 2025.
------------------------------------ */

#if 0
   #define USE_ELF32_TEMPLATE
#endif

#define Swap_Half   swap16
#define Swap_Sword  swap32
#define Swap_Word   swap32
#define Swap_Sxword swap64
#define Swap_Xword  swap64

#if defined(USE_ELF32_TEMPLATE)
   #define RELOCATE relocate32
   #define ELF32_Rel_addend(n) if(!(info->is_rela)) tuim_load(ctx, A, P, n);
   #define Elf(x) Elf32_##x
   #define Swap_Addr swap32
#elif defined(USE_ELF64_TEMPLATE)
   #define RELOCATE relocate64
   #define ELF32_Rel_addend(n)
   #define Elf(x) Elf64_##x
   #define Swap_Addr swap64
#endif

#define LINK_ADDRESS(info, v_addr) \
   ((info)->program_image + ((v_addr) - (info)->start_vaddr))


const Elf32_Byte * RELOCATE(
   const  tuim_ctx *ctx,
   void *ptr,
   const void *_rel, const void *_dep){

   struct tuim_backend * const info  = ptr;
   struct tuim_backend * const * dep = _dep;
   const Elf(Rel)  * const rel    = _rel;
   const Elf(Rela) * const rela   = _rel;
   const Elf(Sym)  * const dynsym = info->dynsym;
   const Elf(Byte) ascii_null = 0;

   const Elf(Sym)  * sym;

   const Elf(Byte) * symbol;
   Elf(Half) st_shndx;
   alignas(int64_t) Elf(Byte) A[8];
   alignas(uint64_t) Elf(Byte) result[8];
   #if defined(USE_ELF32_TEMPLATE)
      Elf32_Word r_info, st_size;
   #elif defined(USE_ELF64_TEMPLATE)
      Elf64_Xword r_info, st_size;
   #endif
   Elf(Addr) r_offset, S, G, L, Z, P, B, GOT;

   int r_type;
   size_t r_sym;

   /* --- initialize relocation variables --- */

   symbol = &ascii_null;

   #if defined(USE_ELF32_TEMPLATE)
      r_info = Swap_Word(rel->r_info);
      r_sym  = ELF32_R_SYM (r_info);
      r_type = ELF32_R_TYPE(r_info);
   #elif defined(USE_ELF64_TEMPLATE)
      r_info = Swap_Xword(rel->r_info);
      r_sym  = ELF64_R_SYM (r_info);
      r_type = ELF64_R_TYPE(r_info);
   #endif

   /* From _SysV ABI AMD64 Architecture Processor Supplement_:
      > P Represents the place (section offset or address) of the
      > storage unit being relocated (computed using r_offset). */
   r_offset = Swap_Addr(rel->r_offset);
   P = LINK_ADDRESS(info, r_offset);

   /* From _SysV ABI AMD64 Architecture Processor Supplement_:
      > A Represents the addend used to compute the value of the relocatable
      > field.

      > The AMD64 LP64 ABI architecture uses only Elf64_Rela relocation
      > entries with explicit addends. The r_addend member serves as the
      > relocation addend.
      > The AMD64 ILP32 ABI architecture uses only Elf32_Rela relocation
      > entries in relocatable files. Executable files or shared objects may
      > use either Elf32_Rela or Elf32_Rel relocation entries. */
   #if defined(USE_ELF32_TEMPLATE)
      if(info->is_rela){
         *((int32_t*)A) = Swap_Sword(rela->r_addend);
      }
   #elif defined(USE_ELF64_TEMPLATE)
      *((int64_t*)A) = Swap_Sxword(rela->r_addend);
   #endif

   /* From _SysV ABI AMD64 Architecture Processor Supplement_:
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
      case(R_X86_64_NONE):
         /* none none */
         break;
      case(R_X86_64_64):
         /* word64, S + A */
         ELF32_Rel_addend(8);
         *((uint64_t*)result) = S + *((int64_t*)A);
         tuim_store(ctx, P, result, 8);
         break;
      case(R_X86_64_PC32):
         /* word32, S + A - P */
         ELF32_Rel_addend(4);
         *((uint32_t*)result) = S + *((int32_t*)A) - P;
         tuim_store(ctx, P, result, 4);
         break;
      case(R_X86_64_GOT32):
         /* word32, G + A */
         break;
      case(R_X86_64_PLT32):
         /* word32, L + A - P */
         break;
      case(R_X86_64_COPY):
         /* none, none */
         break;
      case(R_X86_64_GLOB_DAT):
         /* wordclass, S */
      case(R_X86_64_JUMP_SLOT):
         /* wordclass, S */
         *((Elf(Addr)*)result) = S;
         tuim_store(ctx, P, result, sizeof(Elf(Addr)));
         break;
      case(R_X86_64_GOTPCREL):
         /* word32, G + GOT + A - P */
         break;
      case(R_X86_64_32):
         /* word32, S + A */
      case(R_X86_64_32S):
         /* word32, S + A */
         ELF32_Rel_addend(4);
         *((uint32_t*)result) = S + *((int32_t*)A);
         tuim_store(ctx, P, result, 4);
         break;
      case(R_X86_64_16):
         /* word16, S + A */
         ELF32_Rel_addend(2);
         *((uint16_t*)result) = S + *((int16_t*)A);
         tuim_store(ctx, P, result, 2);
         break;
      case(R_X86_64_PC16):
         /* word16, S + A - P */
         ELF32_Rel_addend(2);
         *((uint16_t*)result) = S + *((int16_t*)A) - P;
         tuim_store(ctx, P, result, 2);
         break;
      case(R_X86_64_8):
         /* word8, S + A */
         ELF32_Rel_addend(1);
         *((uint8_t*)result) = S + *((int8_t*)A);
         tuim_store(ctx, P, result, 1);
         break;
      case(R_X86_64_PC8):
         /* word8, S + A - P */
         ELF32_Rel_addend(1);
         *((uint8_t*)result) = S + *((int8_t*)A) - P;
         tuim_store(ctx, P, result, 1);
         break;
      case(R_X86_64_DTPMOD64):
         /* word64 */
         break;
      case(R_X86_64_DTPOFF64):
         /* word64 */
         break;
      case(R_X86_64_TPOFF64):
         /* word64 */
         break;
      case(R_X86_64_TLSGD):
         /* word32 */
         break;
      case(R_X86_64_TLSLD):
         /* word32 */
         break;
      case(R_X86_64_DTPOFF32):
         /* word32 */
         break;
      case(R_X86_64_GOTTPOFF):
         /* word32 */
         break;
      case(R_X86_64_TPOFF32):
         /* word32 */
         break;
      case(R_X86_64_GOTPC32):
         /* word32, GOT + A - P */
         break;
      case(R_X86_64_SIZE32):
         /* word32, Z + A */
         break;
      case(R_X86_64_GOTPC32_TLSDESC):
         /* word32 */
         break;
      case(R_X86_64_TLSDESC_CALL):
         /* none */
         break;
      case(R_X86_64_TLSDESC):
         /* word64×2 */
         break;
      case(R_X86_64_IRELATIVE):
         /* wordclass, indirect (B + A) */
         break;
      case(R_X86_64_GOTPCRELX):
         /* word32, G + GOT + A - P */
         break;
      case(R_X86_64_REX_GOTPCRELX):
         /* word32, G + GOT + A - P */
         break;
      case(R_X86_64_CODE_4_GOTPCRELX):
         /* word32, G + GOT + A - P */
         break;
      case(R_X86_64_CODE_4_GOTTPOFF):
         /* word32 */
         break;
      case(R_X86_64_CODE_4_GOTPC32_TLSDESC):
         /* word32 */
         break;
      case(R_X86_64_CODE_5_GOTPCRELX):
         /* word32, G + GOT + A - P */
         break;
      case(R_X86_64_CODE_5_GOTTPOFF):
         /* word32 */
         break;
      case(R_X86_64_CODE_5_GOTPC32_TLSDESC):
         /* word32 */
         break;
      case(R_X86_64_CODE_6_GOTPCRELX):
         /* word32, G + GOT + A - P */
         break;
      case(R_X86_64_CODE_6_GOTTPOFF):
         /* word32 */
         break;
      case(R_X86_64_CODE_6_GOTPC32_TLSDESC):
         /* word32 */
         break;
      #if defined(USE_ELF32_TEMPLATE)
         case(R_X86_64_RELATIVE):
            /* wordclass, B + A */
            *((Elf32_Addr*)result) = LINK_ADDRESS(*dep, *((int32_t*)A));
            tuim_store(ctx, P, result, sizeof(Elf32_Addr));
            break;
         case(R_X86_64_RELATIVE64):
            /* word64, B + A */
            break;
      #elif defined(USE_ELF64_TEMPLATE)
         case(R_X86_64_RELATIVE):
            /* wordclass, B + A */
            *((Elf64_Addr*)result) = LINK_ADDRESS(*dep, *((int64_t*)A));
            tuim_store(ctx, P, result, sizeof(Elf64_Addr));
            break;
         case(R_X86_64_PC64):
            /* word64, S + A - P */
            break;
         case(R_X86_64_GOTOFF64):
            /* word64, S + A - GOT */
            break;
         case(R_X86_64_SIZE64):
            /* word64, Z + A */
            break;
      #endif
      default:
   }

   return NULL;
}
