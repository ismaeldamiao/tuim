#include "stdio_impl.h"
#include <stdio.h>
#include <tuim/kernel.h>

void *__fmap(FILE *stream){
   void *ptr;

   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      size_t length;
      int prot, flags;
      const off_t offset = (off_t)0;
      off_t position_indicator_bak;

      position_indicator_bak = lseek(stream->fd, offset, SEEK_CUR);
      length = lseek(stream->fd, offset, SEEK_END);
      lseek(stream->fd, position_indicator_bak, SEEK_SET);

      ptr = mmap(NULL, length, prot, flags, stream->fd, offset);
   #endif

   return ptr;
}
