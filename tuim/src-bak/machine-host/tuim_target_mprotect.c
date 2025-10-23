#include <stddef.h>
#include <stdint.h>
#include <tuim.h>

#define PF_X   (1 << 0)
#define PF_W   (1 << 1)
#define PF_R   (1 << 2)

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
      #include <sys/ptrace.h>
      #include <sys/mman.h>
   #endif
#endif

#if   tuim_kernel == tuim_linux || \
      tuim_kernel == tuim_xnu
   int  tuim_target_write(void*,tuim_addr,const void*,tuim_size);
   long tuim_target_jump (void*,tuim_addr);
#endif

#include "asm.h"

void tuim_target_mprotect
(void *ptr, tuim_addr address, tuim_size lenght, int protection){
   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      /* on Unix-like systems memory is protected using the mprotect
         system call, remember that the little excutable already have the code
         to do a system call */
      int tuim_target_registers(void *ptr, void *new, void *old);
      struct backend { pid_t pid; uintptr_t entry; uintptr_t syscall; } *back;
      struct_regs regs;
      int prot;

      back = ptr;
      prot = 0;

      /* set parameters */
      tuim_target_registers(ptr, NULL, regs);
      regs[arg0] = address;
      regs[arg1] = lenght;
      if(protection | PF_X) prot |= PROT_EXEC;
      if(protection | PF_W) prot |= PROT_WRITE;
      if(protection | PF_R) prot |= PROT_READ;
      regs[arg2] = prot;
      regs[system_call] = __NR_mprotect;
      tuim_target_registers(ptr, regs, NULL);

      /* execute the system call */
      tuim_target_jump(ptr, back->syscall);
   #endif
}
