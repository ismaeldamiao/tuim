#include <stddef.h>
#include <stdint.h>

#include <string.h>
#include <signal.h>
#include <setjmp.h>

#include "tuim_impl.h"
#include "list.h"

/* helper functions */
static void einval(tuim_ctx *ctx);
static void efault(tuim_ctx *ctx, const char *program_name);

static thread_local jmp_buf env;
static void sigsegv(int signal);

static thread_local const char *is_that_tmp;
static bool is_that(void *elf);

int tuim_exec(tuim_ctx *ctx, const char *exec_path, char *const argv[], char *const envp[]){
   tuim_elf *elf;
   uint64_t entry;

   (void)envp;

   is_that_tmp = (void*)exec_path;
   elf = list_find(ctx->elf_list, is_that);
   if(elf == NULL){
      einval(ctx);
      return -1;
   }

   entry = ctx->get_symbol(elf, NULL);

   signal( SIGSEGV, sigsegv);
   switch(setjmp(env)){
      case TUIM_SIGSEGV:
         efault(ctx, (argv != NULL ? argv[0] : exec_path));
         signal( SIGSEGV, SIG_DFL);
         return -1;
      break;
   }

   /* TODO: execute initialization functions */
   ctx->jump(entry);
   /* TODO: execute termination function */

   return -1;
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

static void sigsegv(int signal){
   (void)signal;
   longjmp(env, TUIM_SIGSEGV);
}

static thread_local const char *is_that_tmp;
static bool is_that(void *elf){
   char *s1 = (void*)(((tuim_elf*)(elf))->file.path);
   return (strcmp((void*)s1, (void*)is_that_tmp) == 0);
}
