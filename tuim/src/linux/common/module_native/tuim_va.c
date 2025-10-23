/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 09, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

#include "tuim_fopen.c"

/*******************************************************************************
 * This is the tuim_va() function for the native backend.
 ******************************************************************************/
int tuim_va(int routine, va_list args){
   static const char einval[] = "Unknown routine passed to tuim()";
   tuim_ctx *ctx;

   if(routine == TUIM_NEWCONTEXT){
      const char *tuim_home, *ld_library_path;
      tuim_ctx **ctx;

      (void)va_arg(args, const char *); /* already handled */
      tuim_home = va_arg(args, const char *);
      ld_library_path = va_arg(args, const char *);
      ctx = va_arg(args, tuim_ctx **);
      va_end(args);

      return tuim_newcontext(tuim_home, ld_library_path, ctx);
   }

   /**
    * Except for TUIM_NEWCONTEXT, the first argument is allways a pointer to
    * the tuim_ctx structure.
    */
   ctx = va_arg(args, tuim_ctx *);

   if(routine == TUIM_STRSTATUS){
      const char **str;

      str = va_arg(args, const char **);
      *str = ctx->strerror;
      va_end(args);

      return TUIM_SUCCESS;
   }

   if((routine == TUIM_READ) || (routine == TUIM_WRITE)){
      uintptr_t address;
      size_t size;
      void *buf;

      address = va_arg(args, uintptr_t);
      size = va_arg(args, size_t);
      buf = va_arg(args, void *);
      va_end(args);

      return tuim_read(ctx, address, size, buf);
   }

   // TODO: ALLOC
   // TODO: MPROTECT
   // TODO: REGISTERS

   if(routine == TUIM_LOAD){
      const char *elf_path;

      elf_path = va_arg(args, const char *);
      va_end(args);

      return tuim_load00(ctx, elf_path);
   }

   // TODO: JUMP

   if(routine == TUIM_EXEC){
      const char *exec_path;
      char **argv, **envp;

      exec_path = va_arg(args, const char *);
      argv = va_arg(args, char **);
      envp = va_arg(args, char **);
      va_end(args);

      return tuim_exec(ctx, exec_path, argv, envp);
   }

   ctx->strerror = einval;
   va_end(args);
   return TUIM_EINVAL;
}

#include "tuim_newcontext.c"

#include "tuim_load00.c"
#include "tuim_load01.c"
#include "tuim_load02.c"
#include "tuim_load03.c"

#include "tuim_read.c"
#include "tuim_write.c"
#include "tuim_alloc.c"
#include "tuim_mprotect.c"
#include "tuim_registers.c"

#include "tuim_jump.c"
#include "tuim_exec.c"
