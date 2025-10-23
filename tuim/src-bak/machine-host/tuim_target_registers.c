#include <stddef.h>
#include <tuim.h>

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
      #include <sys/ptrace.h>
   #endif
#endif

int tuim_target_registers(void *ptr, void *new,  void*old){
   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      pid_t pid = *(pid_t*)ptr;

      if(old != NULL){
         ptrace(PTRACE_GETREGS, pid, NULL, old);
      }

      if(new != NULL){
         ptrace(PTRACE_SETREGS, pid, NULL, new);
      }
   #endif

   return TUIM_SUCCESS;
}
