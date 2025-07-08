#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>
#include "../tuim_backend.h"

#define PF_X (1 << 0)
#define PF_W (1 << 1)
#define PF_R (1 << 2)

#if defined(_POSIX_C_SOURCE)
   void
   tuim_mprotect(const tuim_ctx *ctx, Elf(Addr) addr, size_t size, int prot){
      int posix_prot = 0;
      (void)ctx;

      if(prot & PF_X) posix_prot |= PROT_EXEC;
      if(prot & PF_W) posix_prot |= PROT_WRITE;
      if(prot & PF_R) posix_prot |= PROT_READ;

      mprotect((void*)addr, size, posix_prot);
   }
#else
   #error Target OS is not suported
#endif
