/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 06, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

static int tuim_jump(tuim_ctx *ctx, uintptr_t address){
   tuim_registers regs;
   int status[2];
   // see https://www.evilsocket.net/2015/05/01/Dynamically-inject-a-shared-library-into-a-running-process-on-Android-ARM/


   /* set the return as breakpoint by installing a trap */
   /* set the instruction pointer/program counter */
   status[0] = tuim_getregisters(ctx, &regs);
   if(status[0] != TUIM_SUCCESS) return status[0];

   regs[return_address] = ctx->entry;
   regs[program_counter] = address;

   status[0] = tuim_setregisters(ctx, &regs);
   if(status[0] != TUIM_SUCCESS) return status[0];

   /* execute and wait */

   #ifndef NDEBUG
      (void)tuim_getregisters(ctx, &regs);
      (void)printf("tuim: Jumping to 0x%X, ", regs[program_counter]);
      (void)printf("RA: 0x%X\n", regs[return_address]);
   #endif

   for(;;){
      #ifndef NDEBUG
         (void)ptrace(PTRACE_SYSCALL, ctx->pid, NULL, NULL);
      #else
         (void)ptrace(PTRACE_CONT, ctx->pid, NULL, NULL);
      #endif
      (void)waitpid(ctx->pid, status + 1, __WALL);

      status[0] = tuim_getregisters(ctx, &regs);
      if(status[0] != TUIM_SUCCESS) return status[0];

      if(WIFSTOPPED(status[1])){
         #ifndef NDEBUG
            printf("tuim:    Process %d stoped; PC: 0x%X, signal: %d\n",
               ctx->pid, regs[program_counter], WSTOPSIG(status[1]));
         #endif

         if(regs[program_counter] == ctx->entry){
            /* task done, returned */
            break;
         }

         switch(WSTOPSIG(status[1])){
            case (SIGTRAP | (PTRACE_EVENT_EXIT<<8)):
               /* fatal error, here the context is corrupted */
               return TUIM_EUNKNOWN;
            case SIGSEGV:
               return TUIM_EUNKNOWN;
            case SIGILL:
               return TUIM_EUNKNOWN;
            case SIGTRAP:
               #ifndef NDEBUG
                  (void)printf("tuim:       System call: %u\n",
                     regs[system_call]);
                  (void)ptrace(PTRACE_SYSCALL, ctx->pid, NULL, NULL);
                  (void)waitpid(ctx->pid, status + 1, __WALL);
               #endif
               break;
         }
      }else if(WIFSIGNALED(status[1])){
         /* TODO: hangle child signals */
      }

      status[0] = tuim_getregisters(ctx, &regs);
      if(status[0] != TUIM_SUCCESS) return status[0];
   }

   #ifndef NDEBUG
      printf("tuim:    ... process %d waiting to do a task", ctx->pid);
      puts("");
   #endif

   return TUIM_SUCCESS;
}
