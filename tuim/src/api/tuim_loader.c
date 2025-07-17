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
/* ------------------------------------
   This function load a ELF in a context. The file is placed at host's memory
   address space and segments are placed at target's memory.
   Current implementation use nested functions to divide the task
   in modules. Clang is know to inline all modules when using optimization.

   * Part of Tuim Project.
   * Last modified: July 05, 2025.
------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "tuim_impl.h"

#define SIZE_C(x) ((size_t)x##U)

#define EI_MAG0		0
#define ELFMAG0		0x7f

#define EI_MAG1		1
#define ELFMAG1		'E'

#define EI_MAG2		2
#define ELFMAG2		'L'

#define EI_MAG3		3
#define ELFMAG3		'F'

#define EI_CLASS	4
#define ELFCLASSNONE	0
#define ELFCLASS32	1
#define ELFCLASS64	2

void *fmap(FILE *stream);


static thread_local const char *is_that_tmp;
static bool is_that(void *elf);

/* helper functions */
static void einval(tuim_ctx *ctx);
static void enomem(tuim_ctx *ctx);
static void enoent(tuim_ctx *ctx, const char *elf_path);
static void enoelf(tuim_ctx *ctx, const char *elf_path);
static void eelfbad(tuim_ctx *ctx, const char *elf_path);

/* -------------------------------------------------------------------------- */
void tuim_loader(tuim_ctx *ctx, const void *elf_path){
   struct file_s file;
   const uint8_t *e_ident;
   tuim_elf *elf;
   struct list *list;
   bool is_not_ok;

   file.path = elf_path;

   /* sanity checks */

   is_not_ok = (ctx == NULL);
   is_not_ok = (is_not_ok || (file.path == NULL));

   if(is_not_ok){
      einval(ctx);
      return;
   }

   /* check if already loaded */

   is_that_tmp = file.path;
   if(list_find(ctx->elf_list, is_that) != NULL){
      ctx->status = TUIM_SUCCESS;
      return;
   }

   /* map the file */

   file.stream = fopen(file.path, "rb");
   if(file.stream == NULL){
      enoent(ctx, file.path);
      return;
   }

   file.obj = fmap(file.stream);
   if(file.obj == NULL){
      ctx->status = 255;
      return;
   }

   /* check if the machine can execute that file */
   e_ident = file.obj;
   if((e_ident[EI_MAG0] != ELFMAG0) || (e_ident[EI_MAG1] != ELFMAG1) ||
      (e_ident[EI_MAG2] != ELFMAG2) || (e_ident[EI_MAG3] != ELFMAG3)
   ){
      fclose(file.stream);
      free(file.obj);
      enoelf(ctx, file.path);
      return;
   }

   if(ctx->check_attributes(file.obj) != 0){
      fclose(file.stream);
      free(file.obj);
      eelfbad(ctx, file.path);
      return;
   }

   /* allocate memory to the elf structure */

   elf = malloc(sizeof(tuim_elf) + strlen(file.path) + 1);
   if(elf == NULL){
      fclose(file.stream);
      free(file.obj);
      enomem(ctx);
      return;
   }

   elf->file.path = (void*)((char*)elf + sizeof(tuim_elf));
   elf->file.stream = file.stream;
   elf->file.obj = file.obj;

   strcpy((char*)(elf->file.path), file.path);

   /* load file segment on machine's memory */

   if(ctx->load_segments(ctx, elf) != 0){
      fclose(file.stream);
      free(file.obj);
      free(elf);
      enomem(ctx);
      return;
   }

   /* add the file to the list of ELFs on the machine */

   list = list_add(ctx->elf_list, elf);
   if(list == NULL){
      fclose(file.stream);
      free(file.obj);
      ctx->free(elf->start_vaddr);
      free(elf);
      enomem(ctx);
      return;
   }
   ctx->elf_list = list; /* relevant only at the first time */

   ctx->status = TUIM_SUCCESS;
}
/* -------------------------------------------------------------------------- */
static void einval(tuim_ctx *ctx){
   const char * strings[2];
   strings[0] = "ERROR: Invalid arguments passed to tuim_loader";
   strings[1] = NULL;
   tuim_writeerror(ctx, strings, TUIM_EINVAL);
}
static void enoent(tuim_ctx *ctx, const char *elf_path){
   const char * strings[4];
   strings[0] = "ERROR: ";
   strings[1] = elf_path;
   strings[2] = " not found";
   strings[3] = NULL;
   tuim_writeerror(ctx, strings, TUIM_ENOENT);
}
static void enomem(tuim_ctx *ctx){
   const char * strings[2];
   strings[0] = "ERROR: Not enought memory";
   strings[1] = NULL;
   tuim_writeerror(ctx, strings, TUIM_EINVAL);
}
static void enoelf(tuim_ctx *ctx, const char *elf_path){
   const char * strings[4];
   strings[0] = "ERROR: ";
   strings[1] = elf_path;
   strings[2] = " is not a ELF";
   strings[3] = NULL;
   tuim_writeerror(ctx, strings, TUIM_ENOELF);
}
static void eelfbad(tuim_ctx *ctx, const char *elf_path){
   const char * strings[4];
   strings[0] = "ERROR: ";
   strings[1] = elf_path;
   strings[2] = " was wrong attributes";
   strings[3] = NULL;
   tuim_writeerror(ctx, strings, TUIM_ENOELF);
}

static thread_local const char *is_that_tmp;
static bool is_that(void *elf){
   const char *s1 = ((tuim_elf*)(elf))->file.path;
   return (strcmp(s1, is_that_tmp) == 0);
}
