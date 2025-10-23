/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 07, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

/*******************************************************************************
 * Function to get the state of register from the execution context.
 ******************************************************************************/
static int tuim_getregisters(tuim_ctx *ctx, tuim_registers *regs){
   void *data = &((*regs)[0]);
   if(ptrace(PTRACE_GETREGS, ctx->pid, data, data) == -1){
      ctx->strerror = strerror(errno);
      return TUIM_EUNKNOWN;
   }
   return TUIM_SUCCESS;
}

/*******************************************************************************
 * Function to set the state of register from the execution context.
 ******************************************************************************/
static int tuim_setregisters(tuim_ctx *ctx, tuim_registers *regs){
   void *data = &((*regs)[0]);
   if(ptrace(PTRACE_SETREGS, ctx->pid, data, data) == -1){
      ctx->strerror = strerror(errno);
      return TUIM_EUNKNOWN;
   }
   return TUIM_SUCCESS;
}
