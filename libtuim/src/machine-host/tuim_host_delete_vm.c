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

void tuim_host_delete_vm(void *ptr){
   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      pid_t pid = *(pid_t*)ptr;
      kill(pid, SIGKILL);
   #elif tuim_kernel == tuim_windows_nt
   #endif
}
