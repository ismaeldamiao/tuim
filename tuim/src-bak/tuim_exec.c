#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>

#include "tuim_impl.h"
#include "list.h"

/* helper functions */
static void einval(tuim_ctx *ctx);
static void efault(tuim_ctx *ctx, const char *program_name);

static thread_local const char *is_that_tmp;
static bool is_that(void *elf);

int tuim_exec(tuim_ctx *ctx, const char *exec_path, char *const argv[], char *const envp[]){
   tuim_elf *elf;
   uint64_t entry;
   int status, exit_code;

   if(ctx == NULL){
      einval(ctx);
      return 1;
   }

   is_that_tmp = (void*)exec_path;
   elf = list_find(ctx->elf_list, is_that);
   if(elf == NULL){
      tuim_linker(ctx, exec_path);
      if(ctx->status != TUIM_SUCCESS) return EXIT_FAILURE;
      elf = list_find(ctx->elf_list, is_that);
   }

   /* prepare the stack */

   ctx->exec_init(ctx->backend_pointer, argv, envp);

   /* get the entry point */

   entry = ctx->get_symbol(elf, NULL);

   /* TODO: execute initialization functions */

   /* execute the process */

   status = ctx->jump(ctx->backend_pointer, entry);
   switch(TUIM_STOPSIG(status)){
      case TUIM_SIGILL:
         /* FIXME: bad error description */
      case TUIM_SIGSEGV:
         efault(ctx, exec_path);
   }

   /* TODO: execute termination function */

   ctx->exec_fini(ctx->backend_pointer, &exit_code);

   return exit_code;
}

static void einval(tuim_ctx *ctx){
   const char * strings[2];
   strings[0] = "ERROR: Invalid arguments passed to tuim_jump";
   strings[1] = NULL;
   tuim_writeerror(ctx, strings, TUIM_EINVAL);
}
static void efault(tuim_ctx *ctx, const char *program_name){
   const char * strings[3];
   strings[0] = program_name;
   strings[1] = ": invalid memory access (segmentation fault)";
   strings[2] = NULL;
   tuim_writeerror(ctx, strings, TUIM_EFAULT);
}

static thread_local const char *is_that_tmp;
static bool is_that(void *elf){
   char *s1 = (void*)(((tuim_elf*)(elf))->file.path);
   return (strcmp((void*)s1, (void*)is_that_tmp) == 0);
}
