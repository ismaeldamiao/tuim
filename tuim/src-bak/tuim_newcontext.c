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
   char home[1024];

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
      strcpy(home, tuim_home);
   }

   if(ld_library_path == NULL){ ctx->ld_library_path[0] = '\0'; }else{
      strcpy(ctx->ld_library_path, ld_library_path);
   }

   if(machine == NULL){
      ctx->new_vm = tuim_target_new_vm;
      ctx->delete_vm = tuim_target_delete_vm;

      ctx->read = tuim_target_read;
      ctx->write = tuim_target_write;

      ctx->aligned_alloc = tuim_target_aligned_alloc;
      ctx->mprotect = tuim_target_mprotect;
      ctx->free = tuim_free;

      ctx->registers = tuim_target_registers;

      ctx->null = tuim_target_nullptr;
      ctx->check_attributes = tuim_check_attributes;
      ctx->load_segments = tuim_load_segments;
      ctx->get_dependency = tuim_get_dependency;
      ctx->get_relocation = tuim_get_relocation;
      ctx->relocate = tuim_relocate;

      ctx->jump = tuim_target_jump;
      ctx->exec_init = tuim_target_exec_init;
      ctx->exec_fini = tuim_target_exec_fini;

      ctx->get_sym = tuim_get_sym;
      ctx->get_symbol = tuim_get_symbol;
   }else{
      const char * const strings[] = {
         "ERROR: Feature not implemented yet",
         NULL
      };
      tuim_writeerror(ctx, strings, TUIM_EINVAL);

      strcat(home, "/");
      strcat(home, machine);
      #if 0
      char backend_object[512];
      FILE *tuim_so;
      sprintf(backend_object, "%s/tuim.so", tuim_home);
      tuim_so = fopen(backend_object, "rb");
      fclose(tuim_so);
      #endif
   }

   ctx->new_vm(&(ctx->backend_pointer), home);
   if(ctx->backend_pointer == NULL){
      const char * const strings[] = {
         "ERROR: Machine not initialized.",
         NULL
      };
      tuim_writeerror(ctx, strings, TUIM_ENOMEM);
      return ctx;
   }

   ctx->status = TUIM_SUCCESS;
   return ctx;
}
