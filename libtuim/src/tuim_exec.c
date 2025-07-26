#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "tuim_impl.h"
#include "list.h"

#define STACK_SIZE UINT64_C(2048)

/* helper functions */
static void einval(tuim_ctx *ctx);
static void efault(tuim_ctx *ctx, const char *program_name);

static thread_local jmp_buf env;
static void sigsegv(int signal);

static thread_local const char *is_that_tmp;
static bool is_that(void *elf);

int tuim_exec(tuim_ctx *ctx, const char *exec_path, char *const argv[], char *const envp[]){
   tuim_elf *elf;
   uint64_t entry, stack, stack_begin, len, ptr;
   int argc, i;
   char * const *av, * const *ap;
   const uint64_t align_to = 8;

   (void)envp;

   if(ctx == NULL){
      return 1;
   }

   is_that_tmp = (void*)exec_path;
   elf = list_find(ctx->elf_list, is_that);
   if(elf == NULL){
      tuim_linker(ctx, exec_path);
      if(ctx->status != TUIM_SUCCESS) return EXIT_FAILURE;
      elf = list_find(ctx->elf_list, is_that);
   }

   /* allocate memory for the process stack and prepare it */
#if 0
   stack_begin = ctx->malloc(STACK_SIZE);
   if(stack_begin == ctx->null){
      return TUIM_ENOMEM;
   }
   stack = stack_begin + STACK_SIZE - 1;

   for(ap = envp; ap != NULL; ++ap){
      len = strlen(*ap) + 1;
      stack -= len;
      ctx->store(stack, *ap, len);
   }

   for(av = argv, argc = 0; av != NULL; ++av){
      ++argc;
      len = strlen(*av) + 1;
      stack -= len;
      ctx->store(stack, *av, len);
   }

   stack = stack & ~((ctx->sizeof_ptr)-1);

   stack -= ctx->sizeof_ptr;
   ctx->store(stack, &(ctx->null), ctx->sizeof_ptr); // FIXME: endianness problem
   for(ap = envp, len = 0; ap != NULL; ++ap){
      len += strlen(*ap) + 1;
      ptr = stack_begin - len;
      stack -= ctx->sizeof_ptr;
      ctx->store(stack, &ptr, ctx->sizeof_ptr); // FIXME: endianness problem
   }

   stack -= ctx->sizeof_ptr;
   ctx->store(stack, &(ctx->null), ctx->sizeof_ptr); // FIXME: endianness problem
   for(i = argc-1; i >= 0; --i){
      len += strlen(argv[i]) + 1;
      ptr = stack_begin - len;
      stack -= ctx->sizeof_ptr;
      ctx->store(stack, &ptr, ctx->sizeof_ptr); // FIXME: endianness problem
   }

   stack -= ctx->sizeof_int;
   ctx->store(stack, &argc, ctx->sizeof_int); // FIXME: endianness problem
#endif
   /* get the entry point and save the current state */

   entry = ctx->get_symbol(elf, NULL);
#if 0
   signal( SIGSEGV, sigsegv);
   switch(setjmp(env)){
      case TUIM_SIGSEGV:
         efault(ctx, (argv != NULL ? argv[0] : exec_path));
         signal( SIGSEGV, SIG_DFL);
         return TUIM_EFAULT;
      break;
   }
#endif

   /* TODO: execute initialization functions */

   /* execute the process */

   ctx->jump(entry);
   // ctx->jump(entry, stack);

   /* TODO: execute termination function */

   return 255;
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
