#ifndef STDIO_IMPL_H
#define STDIO_IMPL_H

#include <stdio.h>

#include <tuim/kernel.h>

struct __file {
   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      int fd;
   #else
      #error Target kernel is not supported
   #endif
};

#endif /* STDIO_IMPL_H */
