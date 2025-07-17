/* *****************************************************************************
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
/* ------------------------------------
   Function to create a new intance of tuim_ctx.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#include <string.h>
#include <stdlib.h>

#include "tuim_impl.h"

tuim_ctx *tuim_newcontext(
   const char *tuim_home, const char *ld_library_path, const char *machine
){
   tuim_ctx *ctx;

   /* Allocate memory */

   ctx = malloc(sizeof(tuim_ctx));
   if(ctx == NULL){
      return NULL;
   }
   ctx->elf_list = NULL;
   ctx->can_load_exec = true;

   /* Initialize */

   if(tuim_home == NULL){ ctx->tuim_home[0] = '\0'; }else{
      strcpy(ctx->tuim_home, tuim_home);
   }

   if(ld_library_path == NULL){ ctx->ld_library_path[0] = '\0'; }else{
      strcpy(ctx->ld_library_path, ld_library_path);
   }

   if(machine == NULL){
      ctx->malloc = tuim_malloc;
      ctx->aligned_alloc = tuim_aligned_alloc;
      ctx->mprotect = tuim_mprotect;
      ctx->free = tuim_free;
      ctx->memcpy  = tuim_memcpy;
      ctx->memcpy2 = tuim_memcpy2;
      ctx->memcpy4 = tuim_memcpy4;
      ctx->memcpy8 = tuim_memcpy8;
      ctx->load  = tuim_load;
      ctx->load2 = tuim_load2;
      ctx->load4 = tuim_load4;
      ctx->load8 = tuim_load8;
      ctx->store  = tuim_store;
      ctx->store2 = tuim_store2;
      ctx->store4 = tuim_store4;
      ctx->store8 = tuim_store8;
      ctx->memset  = tuim_memset;
      ctx->memset2 = tuim_memset2;
      ctx->memset4 = tuim_memset4;
      ctx->memset8 = tuim_memset8;
      ctx->null = tuim_nullptr;
      ctx->check_attributes = tuim_check_attributes;
      ctx->load_segments = tuim_load_segments;
      ctx->get_dependency = tuim_get_dependency;
      ctx->get_relocation = tuim_get_relocation;
      ctx->relocate = tuim_relocate;
      ctx->jump = tuim_jump;
      ctx->get_sym = tuim_get_sym;
      ctx->get_symbol = tuim_get_symbol;
   }else{
      const char * const strings[] = {
         "ERROR: Feature not implemented yet",
         NULL
      };
      tuim_writeerror(ctx, strings, TUIM_EINVAL);
   }

   return ctx;
}
