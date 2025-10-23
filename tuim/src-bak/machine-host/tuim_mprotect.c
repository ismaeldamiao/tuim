#include <stdint.h>
#include <stdlib.h>
/* ------------------------------------
   mprotect implementation for the host machine.
   * Part of Tuim Project.
   * Last modified: July 24, 2025.
------------------------------------ */

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 202311L
#if __has_include(<tuim/kernel.h>)
   #define tuim_environment
#endif // __has_include(<tuim/kernel.h>)
#endif /* __STDC_VERSION__ < 202311L */
#endif /* defined(__STDC_VERSION__) */

#if defined(tuim_environment)
   #include <tuim/kernel.h>
#else
   #include "kernel.h"
   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      #include <sys/mman.h>
   #endif
#endif

#define PF_X   (1 << 0)
#define PF_W   (1 << 1)
#define PF_R   (1 << 2)

int mprotect(void *addr, size_t len, int prot);

void tuim_mprotect(uint64_t addr, uint64_t size, int prot){
   void *addr_ = (void*)(intptr_t)addr;
   size_t size_ = (size_t)size;
   int prot_ = 0;

   #if   tuim_kernel == tuim_linux ||\
         tuim_kernel == tuim_xnu
      /* System Calls on POSIX API */
      if(prot & PF_X) prot_ |= PROT_EXEC;
      if(prot & PF_W) prot_ |= PROT_WRITE;
      if(prot & PF_R) prot_ |= PROT_READ;

      mprotect(addr_, size_, prot_);
   #elif tuim_kernel == tuim_windows_nt
      // TODO: Implement this
      prot_ = prot;
   #endif
}
