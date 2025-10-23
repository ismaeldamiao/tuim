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
 * This is the first step for the TUIM_LOAD routine.
 *
 * This function check if the requested file is already loaded, if not then
 * the file open for read and mapped in the memory.
 *
 * If the requested file is not a ELF then the routine is stopped.
 ******************************************************************************/
static int tuim_load00(tuim_ctx *ctx, const char *elf_path){
   static char buf[BUFFER_SIZE];
   Elf(Byte) *e_ident;
   tuim_elf file;
   int status;

   if(elf_path == NULL){
      static const char einval[] = "Invalid argument passed to tuim()";
      ctx->strerror = einval;
      return TUIM_EINVAL;
   }

   #ifndef NDEBUG
      printf("tuim: Trying to load %s\n", elf_path);
   #endif

   /* check if already loaded */
   is_that_tmp = elf_path;
   if(list_find(ctx->elf_list, is_that) != NULL){
      return TUIM_SUCCESS;
   }
   /* open the file */

   status = tuim_fopen(&file, elf_path);
   if(status != TUIM_SUCCESS){
      if(status == TUIM_ENOENT){
         (void)sprintf(buf, "%s: file not found", elf_path);
      }
      ctx->strerror = buf;
      return status;
   }

   e_ident = file.map;

   if((e_ident[EI_MAG0] != ELFMAG0) || (e_ident[EI_MAG1] != ELFMAG1) ||
      (e_ident[EI_MAG2] != ELFMAG2) || (e_ident[EI_MAG3] != ELFMAG3)
   ) return TUIM_ENOELF;

   return tuim_load01(ctx, &file);
}
