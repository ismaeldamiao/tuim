/* *****************************************************************************
   Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the “Software”), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
***************************************************************************** */
#include <stddef.h>
#include <stdint.h>
#include <tuim.h>

/* ------------------------------------
   This function execute the code at the address on the target machine --- in
   that case the host machine itself.
   * Part of Tuim Project.
   * Last modified: July 28, 2025.
------------------------------------ */

#define DEBUG 0

#if __STDC_VERSION__ < 201112L
   #define thread_local
#elif  (__STDC_VERSION__ >= 201112L) && (__STDC_VERSION__ < 202311L)
   #define thread_local _Thread_local
#endif

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 202311L
#if __has_include(<tuim/kernel.h>)
   #define tuim_environment
#endif // __has_include(<tuim/kernel.h>)
#endif /* __STDC_VERSION__ < 202311L */
#endif /* defined(__STDC_VERSION__) */

#if defined(tuim_environment)
   #include <tuim/arch.h>
   #include <tuim/kernel.h>
#else
   #include "arch.h"
   #include "kernel.h"
   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      #include <signal.h>
      #include <stdlib.h>
      #include <unistd.h>
      #include <sys/ptrace.h>
      #include <sys/user.h>
      #include <sys/wait.h>
      #include <sys/types.h>
      #include <stdlib.h>
   #endif
#endif

#if   tuim_kernel == tuim_linux || \
      tuim_kernel == tuim_xnu
   int  tuim_target_write(void*,tuim_addr,const void*,tuim_size);
#endif

#include "asm.h"

int tuim_target_jump(void *ptr, tuim_addr address){

   #if DEBUG
      int printf(const char *, ...), puts(const char *);
      printf("   jumping to 0x%X, ", (uintptr_t)(address));
   #endif

   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      /* on Unix-like systems I need to inject the code for some
         systems calls to allocate memory on the process */
      int tuim_target_registers(void *ptr, void *new, void *old);
      struct backend { pid_t pid; uintptr_t entry; } *back;
      struct_regs regs;
      int status;
      // see https://www.evilsocket.net/2015/05/01/Dynamically-inject-a-shared-library-into-a-running-process-on-Android-ARM/

      /* set the return as breakpint by installing a trap */
      back = ptr;
      tuim_target_registers(ptr, NULL, regs);
      regs[return_address] = back->entry;

      /* set the instruction pointer/program counter */
      regs[program_counter] = (uintptr_t)address;
      tuim_target_registers(ptr, regs, NULL);

      /* execute and wait */

      #if DEBUG
         printf(" RA: 0x%X\n", regs[return_address]);
      #endif

      for(;;){
         ptrace(PTRACE_SYSCALL, back->pid, NULL, NULL);
         waitpid(back->pid, &status, __WALL);
         ptrace(PTRACE_GETREGS, back->pid, NULL, &regs);

         if(WIFSTOPPED(status)){
            #if DEBUG
               printf("   Process %d stoped; PC: 0x%X, signal: %d\n",
                  back->pid, regs[program_counter], WSTOPSIG(status));
            #endif

            if(regs[program_counter] == back->entry){
               /* task done, returned */
               break;
            }

            switch(WSTOPSIG(status)){
               case (SIGTRAP | (PTRACE_EVENT_EXIT<<8)):
                  /* fatal error, here the context is corrupted */
                  return 0xFF00;
               case SIGSEGV:
                  return TUIM_STOPCODE(TUIM_SIGSEGV);
               case SIGILL:
                  return TUIM_STOPCODE(TUIM_SIGILL);
               case SIGTRAP:
                  break;
                  #if DEBUG
                     printf("      System call: %u\n",
                        (unsigned)regs[system_call]);
                     ptrace(PTRACE_SYSCALL, back->pid, NULL, NULL);
                     waitpid(back->pid, &status, __WALL);
                  #endif
            }
         }else if(WIFSIGNALED(status)){
            /* TODO: hangle child signals */
         }
         tuim_target_registers(ptr, NULL, regs);
      }

      #if DEBUG
         printf("   ... process %d waiting to do a task", back->pid);
         puts("");
      #endif

      return TUIM_SUCCESS;
   #endif
}
