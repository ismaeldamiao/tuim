#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../tuim_backend.h"

#if defined(_POSIX_C_SOURCE)
   #include <unistd.h>

   Elf(Addr)
   tuim_aligned_alloc(const tuim_ctx *ctx, size_t alignment, size_t size){
      void *addr;
      (void)ctx;

      if(alignment == SIZE_C(0))
         alignment = sysconf(_SC_PAGE_SIZE);

      if(posix_memalign(&addr, alignment, size) != 0)
         return tuim_nullptr;
      return (Elf(Addr))addr;
   }
#else
   #error Target OS is not suported
#endif
