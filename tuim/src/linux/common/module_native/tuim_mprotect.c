/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 07, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

/*******************************************************************************
 * Function to protect memory on the execution context.
 *
 * Current implementation uses mprotect system call by jumping to the
 * system call instruction on the little image.
 ******************************************************************************/
static int tuim_mprotect(tuim_ctx *ctx, uintptr_t address, size_t size, int prot){
   tuim_registers regs;
   int status, prot_ = 0;

   /**
    * Set parameters.
    */
   status = tuim_getregisters(ctx, &regs); /* Don't remove this! */
   if(status != TUIM_SUCCESS) return status;

   if(prot & PF_X) prot_ |= PROT_EXEC;
   if(prot & PF_W) prot_ |= PROT_WRITE;
   if(prot & PF_R) prot_ |= PROT_READ;

   regs[arg0] = address;
   regs[arg1] = size;
   regs[arg2] = prot_;
   regs[system_call] = __NR_mprotect;

   status = tuim_setregisters(ctx, &regs);
   if(status != TUIM_SUCCESS) return status;

   /**
    * Execute the system call.
    */
   status = tuim_jump(ctx, ctx->syscall);
   if(status != TUIM_SUCCESS) return status;

   return TUIM_SUCCESS;
}
