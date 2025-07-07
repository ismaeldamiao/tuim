#include <stddef.h>

#include "tuim.h"
#include "tuim_ctx.h"
#include "tuim_impl.h"

#include "ascii/ascii.h"
#include "ascii/string.h"
/* ------------------------------------
   Functions to get the path of a executable/library by their name.
   * Part of Tuim Project.
   * Last modified: June 17, 2025.
------------------------------------ */

#if __STDC_VERSION__ < 202311L
#define true 1
#define false 0
typedef int bool;
#endif /* __STDC_VERSION__ < 202311L */

static uint8_t memory_buffer[1024];

#define ASCII_SLASH UINT8_C(0x2f)
static bool haveslash(const uint8_t *str){
   for(; *str != UINT8_C(0x00); ++str)
      if(*str == ASCII_SLASH) return true;
   return false;
}

void *tuim_getpathexec(tuim_ctx *ctx, char *_elf_name){
   uint8_t * const elf_path = memory_buffer;
   uint8_t *elf_name;

   elf_name = ascii(_elf_name);
   if(haveslash(elf_name)){
      ascii_strcpy(elf_path, elf_name);
      ctx->status = TUIM_SUCCESS;
      return string(elf_path);
   }

   if(ctx->tuim_home[0] == UINT8_C(0x00)){
      const char * const strings[] = {
         "Error: Environment variable TUIM_HOME not set",
         NULL
      };
      // FIXME: Bad error code
      tuim_writeerror(ctx, strings, TUIM_ENOELF);
      return NULL;
   }

   ascii_strcpy(elf_path, ctx->tuim_home);

   if(ctx->machine != NULL){
      ascii_strcat(elf_path, ascii("/opt/"));
      ascii_strcat(elf_path, ctx->machine);
   }

   ascii_strcat(elf_path, ascii("/bin/"));
   elf_name = ascii(_elf_name);
   ascii_strcat(elf_path, elf_name);
   ascii_strcat(elf_path, ascii(".elf"));

   ctx->status = TUIM_SUCCESS;
   return elf_path;
}

void *tuim_getpathdyn(tuim_ctx *ctx, char *_elf_name){
   uint8_t * const elf_path = memory_buffer;
   uint8_t *elf_name;

   elf_name = ascii(_elf_name);
   if(haveslash(elf_name)){
      ascii_strcpy(elf_path, elf_name);
      ctx->status = TUIM_SUCCESS;
      return string(elf_path);
   }

   if(ctx->tuim_home[0] == UINT8_C(0x00)){
      const char * const strings[] = {
         "Error: Environment variable TUIM_HOME not set",
         NULL
      };
      // FIXME: Bad error code
      tuim_writeerror(ctx, strings, TUIM_ENOELF);
      return NULL;
   }

   ascii_strcpy(elf_path, ctx->tuim_home);

   if(ctx->machine != NULL){
      ascii_strcat(elf_path, ascii("/opt/"));
      ascii_strcat(elf_path, ctx->machine);
   }

   ascii_strcat(elf_path, ascii("/lib/"));
   elf_name = ascii(_elf_name);
   ascii_strcat(elf_path,elf_name);

   ctx->status = TUIM_SUCCESS;
   return elf_path;
}
