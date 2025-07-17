#include <stdint.h>
#include <stdlib.h>
#if defined(_POSIX_C_SOURCE)
   #include <sys/mman.h>
#endif
/* ------------------------------------
   mprotect implementation for the host machine.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */

#define PF_X (1 << 0)
#define PF_W (1 << 1)
#define PF_R (1 << 2)

int mprotect(void *addr, size_t len, int prot);

void tuim_mprotect(uint64_t addr, uint64_t size, int prot){
   void *addr_ = (void*)(intptr_t)addr;
   size_t size_ = (size_t)size;
   int prot_ = 0;
   #if defined(_POSIX_C_SOURCE)
      /* on POSIX protection are distinct from pseudo-standard */
      if(prot & PF_X) prot_ |= PROT_EXEC;
      if(prot & PF_W) prot_ |= PROT_WRITE;
      if(prot & PF_R) prot_ |= PROT_READ;
   #else
      prot_ = prot;
   #endif
   mprotect(addr_, size_, prot_);
}
