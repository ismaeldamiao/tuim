#include <stddef.h>
#include <string.h>

#include "tuim_impl.h"
/* ------------------------------------
   Functions to get the path of a executable/library by their name.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */

static char memory_buffer[1024];

static bool haveslash(const char *str){
   const unsigned char * str_ = (void*)str;
   const unsigned char slash = '/';
   for(; *str_ != '\0'; ++str_)
      if(*str_ == slash) return true;
   return false;
}

void *tuim_getpathexec(tuim_ctx *ctx, const char *elf_name){
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
      tuim_writeerror(ctx, strings, TUIM_ENOELF);
      return NULL;
   }

   strcpy(elf_path, ctx->tuim_home);

   if(ctx->machine != NULL){
      strcat(elf_path, "/opt/");
      strcat(elf_path, ctx->machine);
   }

   strcat(elf_path, "/bin/");
   strcat(elf_path, elf_name);
   strcat(elf_path, ".elf");

   ctx->status = TUIM_SUCCESS;
   return elf_path;
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
      tuim_writeerror(ctx, strings, TUIM_ENOELF);
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
