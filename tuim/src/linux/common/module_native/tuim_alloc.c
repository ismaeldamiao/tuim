/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 07, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

/*******************************************************************************
 * Function to allocate memory on the execution context.
 *
 * Current implementation uses mmap system call by jumping to the system call
 * instruction on the little image.
 ******************************************************************************/
static int tuim_alloc(tuim_ctx *ctx, size_t alignment, size_t size, uintptr_t *address){
   int status;
   tuim_registers regs;

   /**
    * Set parameters.
    */
   status = tuim_getregisters(ctx, &regs); /* Don't remove this! */
   if(status != TUIM_SUCCESS) return status;

   regs[arg0] = alignment;
   regs[arg1] = size;
   regs[arg2] = PROT_READ | PROT_WRITE | PROT_EXEC;
   regs[arg3] = MAP_PRIVATE | MAP_ANONYMOUS;
   regs[arg4] = -1;
   regs[arg5] = 0;
   regs[system_call] = __NR_mmap;

   status = tuim_setregisters(ctx, &regs);
   if(status != TUIM_SUCCESS) return status;

   /**
    * Execute the system call.
    */
   status = tuim_jump(ctx, ctx->syscall);
   if(status != TUIM_SUCCESS) return status;

   /**
    * Get returned address.
    */
   status = tuim_getregisters(ctx, &regs);
   if(status != TUIM_SUCCESS) return status;

   if((void*)(intptr_t)(regs[arg0]) == MAP_FAILED){
      *address = ctx->null;
      return TUIM_ENOMEM;
   }else{
      *address = (uintptr_t)regs[arg0];
   }

   return TUIM_SUCCESS;
}
