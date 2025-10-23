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

#if   tuim_arch == tuim_riscv32
#elif tuim_arch == tuim_riscv64
#elif tuim_arch == tuim_arm
   #define __NR_mmap       192
   #define __NR_mprotect   125
   #define struct_regs     tuim_registers_arm
   #define arg0            0
   #define arg1            1
   #define arg2            2
   #define arg3            3
   #define arg4            4
   #define arg5            5
   #define system_call     7
   #define stack_pointer   13
   #define return_address  14
   #define program_counter 15
   #define trap_size       4
   #define pointer_size    4
#elif tuim_arch == tuim_aarch64
#elif tuim_arch == tuim_i386
#elif tuim_arch == tuim_amd64
#endif
