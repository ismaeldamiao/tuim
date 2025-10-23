/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 08, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

#define USE_ELF32_TEMPLATE
#include "Elf_getdependency.c"
#include "Elf_getrelocation.c"
#include "Elf_relocate.c"

static char memory_buffer[1024];

static bool haveslash(const char *str){
   const unsigned char * str_ = (void*)str;
   const unsigned char slash = '/';
   for(; *str_ != '\0'; ++str_)
      if(*str_ == slash) return true;
   return false;
}

void *tuim_getpathdyn(tuim_ctx *ctx, const char *elf_name){
   char * const elf_path = memory_buffer;

   if(haveslash(elf_name)){
      strcpy(elf_path, elf_name);
      ctx->status = TUIM_SUCCESS;
      return elf_path;
   }

   if(ctx->tuim_home[0] == '\0'){
      const char * strings[2];
      strings[0] = "Error: Environment variable TUIM_HOME not set";
      strings[1] = NULL;
      /* FIXME: Bad error code */
      //tuim_writeerror(ctx, strings, TUIM_ENOELF);
      return NULL;
   }

   strcpy(elf_path, ctx->tuim_home);

   if(ctx->machine != NULL){
      strcat(elf_path, "/opt/");
      strcat(elf_path, ctx->machine);
   }

   strcat(elf_path, "/lib/");
   strcat(elf_path,elf_name);

   ctx->status = TUIM_SUCCESS;
   return elf_path;
}

#ifndef SIZE_C
   #define SIZE_C(x) ((size_t)x##U)
#endif



/*******************************************************************************
 * This is the fourth step for the TUIM_LOAD routine.
 *
 * This function relocate the requested file and link it with dependencies.
 ******************************************************************************/
static int tuim_load03(tuim_ctx *ctx, tuim_elf *elf){
   tuim_elf *elf_dep;
   const void *rel;
   void **dep = NULL;
   size_t dep_count = SIZE_C(0);
   const char *symbol;
   char *elf_path;
   int status;

   /* load dependencies */

   get_dependency: elf_path = (char*)Elf32_getdependency(elf);
   if(elf_path != NULL){

      #ifndef NDEBUG
         printf("tuim: %s requesting %s\n", elf->path, elf_path);
      #endif

      elf_path = tuim_getpathdyn(ctx, elf_path);
      elf_path = strcpy((void*)(elf->stack), elf_path);

      /* NOTE: recursive call may be not the best solution due to the risk of
         stack overflow */
      status = tuim_load00(ctx, elf_path);
      if(status != TUIM_SUCCESS) return status;

      ++dep_count;
      dep = realloc(dep, dep_count * sizeof(void*) + sizeof(NULL));
      is_that_tmp = elf_path;
      elf_dep = list_find(ctx->elf_list, is_that);
      dep[dep_count-SIZE_C(1)] = elf_dep;
      dep[dep_count] = NULL;

      goto get_dependency;
   }

   /* relocate */
   get_relocation: rel = Elf32_getrelocation(elf);
   if(rel != NULL){
      status = Elf32_relocate(ctx, elf, rel, dep);
      if(status != TUIM_SUCCESS){
         static char enosym[BUFFER_SIZE];
         sprintf(
            enosym,
            "Can't relocate %s, '%s' not found",
            elf->path, ctx->strerror);
         ctx->strerror = enosym;
         return TUIM_ENOSYM;
      }
      goto get_relocation;
   }
   return TUIM_SUCCESS;
}
