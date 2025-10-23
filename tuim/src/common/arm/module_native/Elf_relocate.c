/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 10, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"
#include "Elf_symbol.c"

#define Swap_Half  swap16
#define Swap_Sword swap32
#define Swap_Word  swap32
#define Swap_Addr  swap32

#define LINK_ADDRESS(elf, v_addr) \
   ((elf)->program_image + ((v_addr) - (elf)->start_vaddr))

/*******************************************************************************
 * Function to relocate a program on ARM machines (EM_ARM). For more information
 * read "System V Application Binary Interface" and "ELF for the Arm (R)
 * Architecture".
 *
 * The ctx variable shall points to a valid tuim_ctx structure. Otherwise the
 * behavior is undefined.
 *
 * The elf variable shall points to a valid tuim_elf structure. Otherwise the
 * behavior is undefined.
 *
 * The _rel variable shall points to a valid relocation entry with or without
 * explicit addend.
 *
 * The function return a error code. If a reference can't be solved then
 * ctx->srterror is set to point to the symbol name.
 ******************************************************************************/
int Elf32_relocate(
   tuim_ctx *ctx,
   tuim_elf *elf,
   const void *_rel, const void *_dep){

   tuim_elf * const * dep = _dep;
   const Elf32_Rel  * const rel    = _rel;
   const Elf32_Rela * const rela   = _rel;
   const Elf32_Sym  * const dynsym = elf->dynsym;
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
   P = LINK_ADDRESS(elf, r_offset);

   /* From _ELF for the Arm Architecture_:
      > A is the addend for the relocation.

      > With the exception of R_ARM_COPY all dynamic relocations
      > require that the place being relocated is a
      > word-aligned 32-bit object.

      > If the place is subject to a data-type relocation,
      > the initial value in the place is sign-extended to 32
      > bits. */
   if(r_type == R_ARM_COPY) goto have_addend;
   if(elf->is_rela){
      A = Swap_Sword(rela->r_addend);
   }else{
      if(r_type == R_ARM_JUMP_SLOT){
         /* From _ELF for the Arm Architecture_:
            > In a REL form of this relocation the addend, A, is always 0. */
         A = INT32_C(0);
      }else{
         tuim_read(ctx, P, sizeof(uint32_t), &A);
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
   symbol = elf->dynstr + Swap_Word(sym->st_name);

   #ifndef NDEBUG
      printf("tuim: Trying to relocate '%s' at %s\n", symbol, elf->path);
   #endif

   if(sym->st_shndx != STN_UNDEF){
      /* the symbol is defined here */
      S = LINK_ADDRESS(elf, Swap_Word(sym->st_value));
      /* TODO: Handle st_shndx == SHN_XINDEX */
      goto have_symbol;
   }

   /* the symbol is defined in another place */
   if(*dep == NULL){
      ctx->strerror = (const char*)symbol;
      return TUIM_ENOSYM;
   }

   for(dep = _dep; *dep != NULL; ++dep){
      sym = Elf32_sym(*dep, symbol);
      if(sym == NULL) continue;
      st_shndx = Swap_Half(sym->st_shndx);
      if(st_shndx != STN_UNDEF) break;
   }

   if((sym == NULL) || (st_shndx == STN_UNDEF)){
      ctx->strerror = (const char*)symbol;
      return TUIM_ENOSYM;
   }

   S = LINK_ADDRESS(*dep, Swap_Word(sym->st_value));
   /* TODO: Handle st_shndx == SHN_XINDEX */

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
         //tuim_target_memcpy(ptr, P, S, st_size); // FIXME: No implemention
         break;
      case R_ARM_GLOB_DAT:
         /* (S + A) | T */
         result = (S + A) | T;
         tuim_write(ctx, P, sizeof(uint32_t), &result);
         break;
      case R_ARM_JUMP_SLOT:
         /* (S + A) | T */
         result = (S + A) | T;
         tuim_write(ctx, P, sizeof(uint32_t), &result);
         break;
      case R_ARM_RELATIVE:
         /* B(S) + A [Note: see Dynamic relocations] */
         result = ((S == 0) ? LINK_ADDRESS(elf, A) : LINK_ADDRESS(*dep, A));
         tuim_write(ctx, P, sizeof(uint32_t), &result);
         break;
      case R_ARM_IRELATIVE:
         /* Reserved for future functionality */
         break;
      default:
         (void)1;
   }

   return TUIM_SUCCESS;
}

#undef Swap_Half
#undef Swap_Sword
#undef Swap_Word
#undef Swap_Addr

#undef LINK_ADDRESS
