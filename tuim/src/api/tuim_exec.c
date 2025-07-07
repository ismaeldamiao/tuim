#include <stddef.h>
#include <stdint.h>

#include <signal.h>
#include <setjmp.h>

#include "ascii/ascii.h"
#include "ascii/string.h"
#include "list.h"

#include "tuim.h"
#include "tuim_ctx.h"
#include "tuim_impl.h"

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

/* helper functions */
static void einval(tuim_ctx *ctx);
static void efault(tuim_ctx *ctx, const char *program_name);

static thread_local jmp_buf env;
static void sigsegv(int signal);

static thread_local const uint8_t *is_that_tmp;
static bool is_that(void *elf);

int tuim_exec(tuim_ctx *ctx, const char *exec_path, char *const argv[], char *const envp[]){
   tuim_elf *elf;
   uint64_t entry;

   (void)envp;

   is_that_tmp = ascii(exec_path);
   elf = list_find(ctx->elf_list, is_that);
   if(elf == NULL){
      einval(ctx);
      return -1;
   }

   entry = ctx->get_symbol(ctx, elf->backend_ptr, NULL);

   signal( SIGSEGV, sigsegv);
   switch(setjmp(env)){
      case TUIM_SIGSEGV:
         efault(ctx, (argv != NULL ? argv[0] : exec_path));
         signal( SIGSEGV, SIG_DFL);
         return -1;
      break;
   }

   // TODO: execute initialization functions
   ctx->jump(ctx, entry);
   // TODO: execute termination function

   return -1;
}

static void einval(tuim_ctx *ctx){
   const char * const strings[] = {
      "ERROR: Invalid arguments passed to tuim_jump",
      NULL
   };
   tuim_writeerror(ctx, strings, TUIM_EINVAL);
}
static void efault(tuim_ctx *ctx, const char *program_name){
   const char * const strings[] = {
      program_name,
      ": invalid memory access (segmentation fault)",
      NULL
   };
   tuim_writeerror(ctx, strings, TUIM_EFAULT);
}

static void sigsegv(int signal){
   (void)signal;
   longjmp(env, TUIM_SIGSEGV);
}

static thread_local const uint8_t *is_that_tmp;
static bool is_that(void *elf){
   uint8_t *s1 = ((tuim_elf*)(elf))->identifier.character_array;
   return (ascii_strcmp(s1, is_that_tmp) == 0);
}
