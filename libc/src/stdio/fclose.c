#include "stdio_impl.h"

#include <tuim/kernel.h>

int fclose(FILE *stream){

   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      sys_close(stream->fd);
   #endif

   return 0;
}
