/* *****************************************************************************
   MIT License

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
#include "../tuim.h"
#include "../tuim_impl.h"
#include "../adt.h"
/* ------------------------------------
   This function call the ELF interpreter and execute the program.
   * Part of Tuim Project.
   * Last modified: April 10, 2025.
------------------------------------ */

static bool file_exits(const char *file_path);

int tuim_run(const char *machine, const char *exec_path, char **argv){
   int tuim_errno;
   tuim_elf *exec;
   tuim_ctx *ctx;

   ctx = new(Tuim_ctx, machine);
   if(ctx == NULL){
      return TUIM_ENOMEM;
   }else if(ctx->status != TUIM_SUCCESS){
      return ctx->status;
   }

   exec = tuim_loader(ctx, exec_path);
   if(ctx->status != TUIM_SUCCESS){
      return ctx->status;
   }

   tuim_linker(ctx, exec);
   if(ctx->status != TUIM_SUCCESS){
      return ctx->status;
   }

   tuim_exec(ctx, exec);
   tuim_errno = ctx->status;

   delete(ctx);
   return tuim_errno;
}
