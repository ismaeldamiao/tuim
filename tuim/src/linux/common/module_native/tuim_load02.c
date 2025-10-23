/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 08, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

#define USE_ELF32_TEMPLATE
#include "Elf_load.c"

/*******************************************************************************
 * This is the third step for the TUIM_LOAD routine.
 *
 * This function load the requested file on the execution context.
 ******************************************************************************/
static int tuim_load02(tuim_ctx *ctx, tuim_elf *elf){
   int status;
   void *ptr;
   struct list *list;

   status = Elf32_load(ctx, elf);
   if(status != TUIM_SUCCESS){
      tuim_fclose(elf);
      return status;
   }

   /**
    * At this point the file was successfully loaded. Now put the elf
    * structure on the list of loaded files.
    */
   ptr = malloc(sizeof(tuim_elf));
   if(ptr == NULL){
      static const char enomem[] = "Can't allocate memory";
      tuim_fclose(elf);
      ctx->strerror = enomem;
      return TUIM_ENOMEM;
   }
   *((tuim_elf*)ptr) = *elf;

   list = list_add(ctx->elf_list, ptr);
   if(list == NULL){
      static const char enomem[] = "Can't allocate memory";
      tuim_fclose(elf);
      return TUIM_ENOMEM;
   }
   ctx->elf_list = list; /* relevant only at the first time */

   return tuim_load03(ctx, ptr);
}
