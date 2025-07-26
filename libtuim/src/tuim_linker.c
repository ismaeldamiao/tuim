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
   This function relocate a ELF and all they dependencies.

   * Part of Tuim Project.
   * Last modified: July 05, 2025.
------------------------------------ */
#include <stdlib.h>
#include <string.h>

#include "tuim_impl.h"

#include "list.h"

#define SIZE_C(x) ((size_t)x##U)

static thread_local const char *is_that_tmp;
static bool is_that(void *elf);

/* helper functions */
static void einval(tuim_ctx *ctx);
static void enosym(tuim_ctx *ctx, const char *elf_path, const char *symbol);

/* -------------------------------------------------------------------------- */
void tuim_linker(tuim_ctx *ctx, const char *elf_path){
   tuim_elf *elf, *elf_dep;
   const void *rel;
   void **dep = NULL;
   size_t dep_count = SIZE_C(0);
   const char *symbol;

   /* sanity checks */

   if((ctx == NULL) || (elf_path == NULL)){
      einval(ctx);
      return;
   }

   is_that_tmp = elf_path;
   elf = list_find(ctx->elf_list, is_that);
   if(elf == NULL){
      tuim_loader(ctx, elf_path);
      if(ctx->status != TUIM_SUCCESS) return;
      elf = list_find(ctx->elf_list, is_that);
   }

   /* load dependencies */

   get_dependency: elf_path = (char*)(ctx->get_dependency(elf));
   if(elf_path != NULL){
      elf_path = tuim_getpathdyn(ctx, elf_path);
      tuim_loader(ctx, elf_path);
      if(ctx->status != TUIM_SUCCESS) return;

      ++dep_count;
      dep = realloc(dep, dep_count * sizeof(void*) + sizeof(NULL));
      is_that_tmp = elf_path;
      elf_dep = list_find(ctx->elf_list, is_that);
      dep[dep_count-SIZE_C(1)] = elf_dep;
      dep[dep_count] = NULL;

      /* NOTE: recursive call may be not the best solution due to the risk of
         stack overflow */
      tuim_linker(ctx, elf_path);
      if(ctx->status != TUIM_SUCCESS) return;
      goto get_dependency;
   }

   /* relocate */

   get_relocation: rel = ctx->get_relocation(elf);
   if(rel != NULL){
      symbol = (void*)(ctx->relocate(elf, rel, dep));
      if(symbol != NULL){
         enosym(ctx, elf->file.path, symbol);
         return;
      }
      goto get_relocation;
   }
}
/* -------------------------------------------------------------------------- */
static void einval(tuim_ctx *ctx){
   const char * strings[2] ;
   strings[0] = "ERROR: Invalid arguments passed to tuim_linker";
   strings[1] = NULL;
   tuim_writeerror(ctx, strings, TUIM_EINVAL);
}

static void enosym(tuim_ctx *ctx, const char *elf_path, const char *symbol){
   const char * strings[6];
   strings[0] = "ERROR: Can't relocate ";
   strings[1] = elf_path;
   strings[2] = ", symbol `";
   strings[3] = symbol;
   strings[4] = "` not found";
   strings[5] = NULL;
   tuim_writeerror(ctx, strings, TUIM_ENOSYM);
}

static thread_local const char *is_that_tmp;
static bool is_that(void *elf){
   const char *s1 = ((tuim_elf*)(elf))->file.path;
   return (strcmp(s1, is_that_tmp) == 0);
}
