#include <stddef.h>

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
#endif

void *tuim_host_new_vm(void){

   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      /* on unix-like systems that have ptrace the process is
         forked and the child is traced by the parent */
      static thread_local pid_t pid;
      pid = fork();
      if(pid < (pid_t)0){
         return NULL;
      }

      if(pid == (pid_t)0){ /* child */
         ptrace(PTRACE_TRACEME);
         kill(getpid(), SIGSTOP);
      }else{ /* parent */
         return &pid;
      }
   #elif tuim_kernel == tuim_windows_nt
   #endif

   return NULL;
}
