/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 06, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

#if !defined(USE_ELF32_TEMPLATE) && !USE_ELF64_TEMPLATE
   #define USE_ELF32_TEMPLATE
#endif

#if defined(USE_ELF32_TEMPLATE)
   #define Elf(x)    Elf32_##x
#elif defined(USE_ELF64_TEMPLATE)
   #define Elf(x)    Elf64_##x
#endif

/*******************************************************************************
 * This is the second step for the TUIM_LOAD routine.
 *
 * This function check if the requested file have the correct attributes for
 * the execution machine.
 ******************************************************************************/
static int tuim_load01(tuim_ctx *ctx, tuim_elf *file){
   Elf(Byte) *e_ident;
   Elf(Ehdr) *ehdr;

   ehdr = file->map;
   e_ident = ehdr->e_ident;

   if(e_ident[EI_DATA] != ELFDATA2LSB) return TUIM_EELFBAD;
   if(e_ident[EI_CLASS] != ELFCLASS32) return TUIM_EELFBAD;

   #ifndef NDEBUG
      if(ehdr->e_type == ET_EXEC) puts("tuim:    ELF type: EXEC");
      else if(ehdr->e_type == ET_DYN) puts("tuim:    ELF type: DYN");
   #endif

   if(ctx->not_expecting_exec && (ehdr->e_type == ET_EXEC)){
      tuim_fclose(file);
      return TUIM_EELFBAD;
   }

   if(ehdr->e_machine != EM_ARM) return TUIM_EELFBAD;
   if((ehdr->e_flags & EF_ARM_EABI_VER5) == 0) return TUIM_EELFBAD;
   #if __ARM_ARCH < 7
      /* armv7l-unknown-unknown-eabi */
      if((ehdr->e_flags & EF_ARM_ABI_FLOAT_SOFT) == 0) return TUIM_EELFBAD;
   #endif

   return tuim_load02(ctx, file);
}
