/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 06, 2025.
 ******************************** MIT licensed ********************************/
#include <stddef.h> /** Use only ANSI C headers and <tuim.h>. */
#include <stdarg.h>
#include <tuim.h>

typedef int (tuim_f)(int routine, va_list args);
extern tuim_f tuim_va;

/**
 * Any implementation backend shall put a pointer to the tuim_va() function
 * at the begin of a tuim_ctx structure, this allow the abstraction neccessary
 * for the tuim() function bellow.
 */
struct tuim_ctx {
   tuim_f *tuim_va;
};

/*******************************************************************************
 * This is the reference implementation for the tuim() function.
 *
 * For a detailed description see the appendex "C Application Programing
 * Interface" of "Tuim: A Portable Application Binary Interface".
 ******************************************************************************/
int tuim(int routine, ...){
   va_list args, args_copy;
   int status;

   void *ptr;
   struct tuim_ctx *ctx;
   char *backend;

   /**
    * The tuim_va() function shall call va_end().
    */
   va_start(args, routine);

   va_copy(args_copy, args);
   ptr = va_arg(args_copy, void *);
   va_end(args_copy);

   /**
    * There are few special cases where the ctx may be a null pointer:
    * - When TUIM_NEWCONTEXT fail by not enough memory.
    */
   if((routine == TUIM_STRSTATUS) && (ptr == NULL)){
      static const char enomem[] = "Can't allocate memory";
      const char **str;
      (void)va_arg(args, void*);
      str = va_arg(args, const char **);
      *str = enomem;
      va_end(args);
      return TUIM_SUCCESS;
   }

   /**
    * Execept when calling TUIM_NEWCONTEXT, the tuim_va() function from the
    * backend implementation shall do the task.
    */
   if(routine != TUIM_NEWCONTEXT){
      ctx = ptr;
      return ctx->tuim_va(routine, args);
   }

   /**
    * When the first argument is null the tuim_va() function from the native
    * backend shall be called to create a new execution context.
    */
   if(ptr == NULL){
      return tuim_va(routine, args);
   }

   /**
    * Otherwise the requested backend shall be loaded using the native backend
    * and then used to create a new execution context.
    */
   #if 0
   char so_path[512];
   uintptr_t address;
   tuim_f tuim_va;

   if(machine_so[0] == '/'){
      strcpy(so_path, tuim_home);
      strcat(so_path, machine_so);
      machine_so = so_path;
   }
   status = tuim(TUIM_LOAD, tuim_thiscontext, machine_so);
   if(status != TUIM_SUCCESS) return status;
   status = tuim(TUIM_SYMBOL, tuim_thiscontext, "tuim_va", &address);
   if(status != TUIM_SUCCESS) return status;
   tuim_va = (tuim_f)(void*)address;

   /* call backend */
   return tuim_va(routine, args);
   #endif

   /* Expected to be never executed. */
   return TUIM_EINVAL;
}
