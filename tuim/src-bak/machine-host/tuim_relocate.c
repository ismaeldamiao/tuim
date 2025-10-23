#if __STDC_VERSION__ < 202311L
   #include "arch.h"
#else
   #if !__has_include(<tuim/arch.h>)
      #include "arch.h"
   #else
      #include <tuim/arch.h>
   #endif
#endif

#if   tuim_arch == tuim_riscv32
   #error Not implemented yet
#elif tuim_arch == tuim_riscv64
   #error Not implemented yet
#elif tuim_arch == tuim_arm
   #include "relocate-arm.c"
#elif tuim_arch == tuim_aarch64
   #include "relocate-aarch64.c"
#elif tuim_arch == tuim_i386
   #error Not implemented yet
#elif tuim_arch == tuim_amd64
   #include "relocate-amd64.c"
#endif
