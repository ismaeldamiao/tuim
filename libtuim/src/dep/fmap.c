#if __STDC_VERSION__ < 202311L
   #include "kernel.h"
#else
   #if !__has_include(<tuim/arch.h>)
      #include "kernel.h"
   #else
      #include <tuim/kernel.h>
   #endif
#endif

#if   tuim_kernel == tuim_linux ||\
      tuim_kernel == tuim_xnu
   #include "fmap-posix.c"
#elif defined(_WIN32)
   #include "fmap-win.c"
#endif
