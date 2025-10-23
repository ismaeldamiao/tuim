#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 202311L
#if __has_include(<tuim/kernel.h>)
   #define tuim_environment
#endif // __has_include(<tuim/kernel.h>)
#endif /* __STDC_VERSION__ < 202311L */
#endif /* defined(__STDC_VERSION__) */

#define TUIM_KERNEL_PROTOTYPES
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
   #endif
#endif

void tuim_target_delete_vm(void *ptr){
   if(ptr == NULL) return;

   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      pid_t pid = *(pid_t*)ptr;
      kill(pid, SIGKILL);
      free(ptr);
   #elif tuim_kernel == tuim_windows_nt
   #endif
}
