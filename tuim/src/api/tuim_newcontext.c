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
#include <stdint.h>
#include <stdlib.h>

#include "ascii/ascii.h"
#include "ascii/string.h"

#include "../backend/tuim_backend.h"
#include "tuim.h"
#include "tuim_ctx.h"
#include "tuim_impl.h"
/* ------------------------------------
   Function to create a new intance of tuim_ctx.
   * Part of Tuim Project.
   * Last modified: July 05, 2025.
------------------------------------ */

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

   if(tuim_home == NULL) ctx->tuim_home[0] = UINT8_C(0x00); else{
      const uint8_t *env = ascii(tuim_home);
      ascii_strcpy(ctx->tuim_home, env);
   }

   if(ld_library_path == NULL) ctx->ld_library_path[0] = UINT8_C(0x00); else{
      const uint8_t *env = ascii(ld_library_path);
      ascii_strcpy(ctx->ld_library_path, env);
   }

   if(machine == NULL){
      ctx->have_correct_attributes = tuim_attributes;
      ctx->get_dependency = tuim_get_dependency;
      ctx->load_segments = tuim_load_segments;
      ctx->get_relocation = tuim_get_relocation;
      ctx->relocate = tuim_relocate;
      ctx->get_symbol = tuim_get_symbol;
      ctx->jump = tuim_jump;
   }else{
      const char * const strings[] = {
         "ERROR: Feature not implemented yet",
         NULL
      };
      tuim_writeerror(ctx, strings, TUIM_EINVAL);
   }

   return ctx;
}
