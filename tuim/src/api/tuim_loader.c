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

#include "list.h"
#include "ascii/ascii.h"
#include "ascii/string.h"

#include "tuim.h"
#include "tuim_ctx.h"
#include "tuim_impl.h"
/* ------------------------------------
   This function load a ELF in a context. The file is placed at host's memory
   address space and segments are placed at target's memory.
   Current implementation use nested functions to divide the task
   in modules. Clang is know to inline all modules when using optimization.

   * Part of Tuim Project.
   * Last modified: July 05, 2025.
------------------------------------ */

#if __STDC_VERSION__ < 202311L
   #define true 1
   #define false 0
#endif
#if __STDC_VERSION__ < 199901L
   typedef int bool;
#elif (__STDC_VERSION__ >= 199901L) && (__STDC_VERSION__ < 202311L)
   typedef _Bool bool;
#endif

#if __STDC_VERSION__ < 201112L
   #define thread_local
#elif  (__STDC_VERSION__ >= 201112L) && (__STDC_VERSION__ < 202311L)
   #define thread_local _Thread_local
#endif

#define SIZE_C(x) ((size_t)x##U)

#define EI_MAG0		0
#define ELFMAG0		0x7f

#define EI_MAG1		1
#define ELFMAG1		'E'

#define EI_MAG2		2
#define ELFMAG2		'L'

#define EI_MAG3		3
#define ELFMAG3		'F'

/* This structure is used to comunicate common variables within the functions
   commented as "control flow" */
struct tuim_loader_variables {
   tuim_ctx *ctx;
   tuim_elf *elf;
};

/* helper functions */
static void unmap_file(struct tuim_loader_variables *arg);
static void einval(tuim_ctx *ctx);
static void enomem(tuim_ctx *ctx);
static void enoelf(tuim_ctx *ctx, const uint8_t *elf_path);
static void eelfbad(tuim_ctx *ctx, const uint8_t *elf_path);

/* control flow */
static void sanity_checks(struct tuim_loader_variables*);
static void check_if_already_loaded(struct tuim_loader_variables *);
static void map_file(struct tuim_loader_variables*);
static void check_if_machine_can_execute(struct tuim_loader_variables*);
static void load_segments(struct tuim_loader_variables*);
static void add_elf_to_the_list(struct tuim_loader_variables*);

/* -------------------------------------------------------------------------- */
void tuim_loader(tuim_ctx *ctx, const void *elf_path){
   struct tuim_loader_variables arg;
   tuim_elf elf_tmp;

   DBG("LOG: %s was loaded at ", string(elf_path));

   arg.ctx = ctx;
   arg.elf = &elf_tmp;
   elf_tmp.identifier.character_array = (void*)elf_path;

   sanity_checks(&arg);
}
/* -------------------------------------------------------------------------- */

static void einval(tuim_ctx *ctx){
   const char * const strings[] = {
      "ERROR: Invalid arguments passed to tuim_loader",
      NULL
   };
   tuim_writeerror(ctx, strings, TUIM_EINVAL);
}
static void enoent(tuim_ctx *ctx, const uint8_t *elf_path){
   const char * const strings[] = {
      "ERROR: ",
      string(elf_path),
      " not found",
      NULL
   };
   tuim_writeerror(ctx, strings, TUIM_ENOENT);
}
static void enomem(tuim_ctx *ctx){
   const char * const strings[] = {
      "ERROR: Not enought memory",
      NULL
   };
   tuim_writeerror(ctx, strings, TUIM_EINVAL);
}
static void enoelf(tuim_ctx *ctx, const uint8_t *elf_path){
   const char * const strings[] = {
      "ERROR: ",
      string(elf_path),
      " is not a ELF",
      NULL
   };
   tuim_writeerror(ctx, strings, TUIM_ENOELF);
}
static void eelfbad(tuim_ctx *ctx, const uint8_t *elf_path){
   const char * const strings[] = {
      "ERROR: ",
      string(elf_path),
      " was wrong attributes",
      NULL
   };
   tuim_writeerror(ctx, strings, TUIM_ENOELF);
}
static thread_local const uint8_t *is_that_tmp;
static bool is_that(void *elf){
   uint8_t *s1 = ((tuim_elf*)(elf))->identifier.character_array;
   return (ascii_strcmp(s1, is_that_tmp) == 0);
}


static void sanity_checks(struct tuim_loader_variables *arg){
   tuim_ctx *ctx = arg->ctx;
   tuim_elf *elf = arg->elf;
   bool is_not_ok;

   elf->identifier.len = ascii_strlen(elf->identifier.character_array);
   is_not_ok = (arg->ctx == NULL);
   is_not_ok = (is_not_ok || (elf->identifier.character_array == NULL));
   is_not_ok = (is_not_ok || (elf->identifier.len == SIZE_C(0)));

   if(is_not_ok){
      einval(ctx);
      return;
   }
   check_if_already_loaded(arg);
}

/* if the object can be found on context's list of objects the there is nothing
   to do */
static void check_if_already_loaded(struct tuim_loader_variables *arg){
   tuim_ctx *ctx = arg->ctx;
   tuim_elf *elf = arg->elf;

   is_that_tmp = elf->identifier.character_array;
   if(list_find(ctx->elf_list, is_that) != NULL){
      ctx->status = TUIM_SUCCESS;
      return;
   }

   map_file(arg);
}

#if defined(_POSIX_C_SOURCE) /* --------------------------------------------- */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

static void map_file(struct tuim_loader_variables *arg){
   tuim_ctx *ctx = arg->ctx;
   tuim_elf *elf = arg->elf;
   struct file_s *file = &(arg->elf->file);

   int fd;
   char *filename;
   struct stat s;
   const off_t offset = (off_t)0;

   filename = string(elf->identifier.character_array);
   file->obj = NULL;

   fd = open(filename, O_RDONLY);
   if(fd == -1){
      enoent(ctx, elf->identifier.character_array);
      return;
   }

   fstat(fd, &s);
   file->sz = s.st_size;
   file->obj = mmap(
      NULL, file->sz, PROT_READ, MAP_PRIVATE, fd, offset
   );
   close(fd);
   if(file->obj == MAP_FAILED){
      file->obj = NULL;
      enomem(ctx);
      return;
   }

   check_if_machine_can_execute(arg);
}

static void unmap_file(struct tuim_loader_variables *arg){
   if(arg->elf->file.obj != NULL)
      munmap(arg->elf->file.obj, arg->elf->file.sz);
}

#elif defined(_WIN32) /* ---------------------------------------------------- */
#error Not implemented yet
#else
#error Unknown target OS
#endif /* defined(_POSIX_C_SOURCE) ------------------------------------------ */

static void check_if_machine_can_execute(struct tuim_loader_variables *arg){
   tuim_ctx *ctx = arg->ctx;
   struct file_s *file = &(arg->elf->file);

   if(!(ctx->have_correct_attributes(file->obj))){
      unmap_file(arg);
      eelfbad(ctx, arg->elf->identifier.character_array);
      return;
   }

   load_segments(arg);
}

static void load_segments(struct tuim_loader_variables *arg){
   tuim_ctx *ctx = arg->ctx;
   tuim_elf *elf = arg->elf;
   struct file_s *file = &(arg->elf->file);
   const uint8_t * const e_ident = (uint8_t*)(file->obj);

   if((e_ident[EI_MAG0] != ELFMAG0) || (e_ident[EI_MAG1] != ELFMAG1) ||
      (e_ident[EI_MAG2] != ELFMAG2) || (e_ident[EI_MAG3] != ELFMAG3)
   ){
      unmap_file(arg);
      enoelf(ctx, elf->identifier.character_array);
      return;
   }

   if(ctx->load_segments(ctx, file->obj, &(elf->backend_ptr)) != 0){
      unmap_file(arg);
      enomem(ctx);
      return;
   }

   add_elf_to_the_list(arg);
}

static void add_elf_to_the_list(struct tuim_loader_variables *arg){
   tuim_ctx *ctx = arg->ctx;
   uint8_t *filename = arg->elf->identifier.character_array;
   size_t len = arg->elf->identifier.len;

   struct list *list;
   tuim_elf *elf;

   /* create the elf structure */
   elf = malloc(sizeof(tuim_elf) + len + 1);
   if(elf == NULL){
      unmap_file(arg);
      enomem(ctx);
      return;
   }

   /* copy the file name, memory reserved for it is just after the
      memory reserved for the elf structure */
   arg->elf->identifier.character_array = (uint8_t*)(void*)(
      (char*)elf + sizeof(tuim_elf));
   ascii_strcpy(arg->elf->identifier.character_array, filename);

   /* initialize the elf structure */
   *elf = *(arg->elf);

   /* add the elf structure to the list in the context */
   list = list_add(arg->ctx->elf_list, elf);
   if(list == NULL){
      free(elf);
      unmap_file(arg);
      enomem(ctx);
      return;
   }
   ctx->elf_list = list; /* relevant only at the first time */

   ctx->status = TUIM_SUCCESS;
}
