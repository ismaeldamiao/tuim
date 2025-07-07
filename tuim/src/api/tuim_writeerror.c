#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "tuim.h"
#include "tuim_ctx.h"
/* ------------------------------------
   This is a helper function to write easily the strerror.
   * Part of Tuim Project.
   * Last modified: June 17, 2025.
------------------------------------ */

#if __STDC_VERSION__ < 201112L
   #define thread_local
#elif  (__STDC_VERSION__ >= 201112L) && (__STDC_VERSION__ < 202311L)
   #define thread_local _Thread_local
#endif

static thread_local char memory_buffer[2048];

void tuim_writeerror(tuim_ctx *ctx, const char * const strings[], int status){
   memory_buffer[0] = '\0';
   while(*strings != NULL){
      strcat(memory_buffer, *strings);
      ++strings;
   }
   ctx->strerror = memory_buffer;
   ctx->status = status;
   return;
}
