#include <stddef.h>
#include <stdint.h>
#include <tuim.h>

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

const tuim_addr tuim_target_nullptr = (tuim_addr)(uintptr_t)NULL;

tuim_addr tuim_target_aligned_alloc
(void *ptr, tuim_size alignment, tuim_size size){
   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      /* on Unix-like systems memory is allocated using the mmap system call,
         remember that the little excutable already have the code
         to do a system call */
      int tuim_target_registers(void *ptr, void *new, void *old);
      struct backend { pid_t pid; uintptr_t entry; uintptr_t syscall; } *back;
      struct_regs regs;

      back = ptr;

      /* set parameters */
      tuim_target_registers(ptr, NULL, regs);
      regs[arg0] = (long)alignment;
      regs[arg1] = (long)size;
      regs[arg2] = PROT_READ | PROT_WRITE | PROT_EXEC;
      regs[arg3] = MAP_PRIVATE | MAP_ANONYMOUS;
      regs[arg4] = -1;
      regs[arg5] = 0;
      regs[system_call] = __NR_mmap;
      tuim_target_registers(ptr, regs, NULL);

      /* execute the system call */
      tuim_target_jump(ptr, back->syscall);

      /* get returned address */
      ptrace(PTRACE_GETREGS, back->pid, NULL, &regs);
      if((void*)(intptr_t)regs[arg0] == MAP_FAILED)
         return tuim_target_nullptr;
      else
         return (tuim_addr)regs[arg0];
   #endif
}
