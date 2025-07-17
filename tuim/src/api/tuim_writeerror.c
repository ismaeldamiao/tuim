#include <stddef.h>
/* ------------------------------------
   This is a helper function to write easily the strerror.
   * Part of Tuim Project.
   * Last modified: June 17, 2025.
------------------------------------ */
#include <string.h>

#include "tuim_impl.h"

static thread_local char memory_buffer[2048];

void tuim_writeerror(
   struct tuim_ctx_s *ctx,
   const char * const strings[],
   int status
){
   memory_buffer[0] = '\0';
   while(*strings != NULL){
      strcat(memory_buffer, *strings);
      ++strings;
   }
   ctx->strerror = memory_buffer;
   ctx->status = status;
   return;
}
