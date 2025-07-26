#include <stdlib.h>
#include "stdio_impl.h"


#include <tuim/kernel.h>

FILE *fopen(const char *filename, const char *mode){
   FILE *f;

   f = malloc(sizeof(FILE));
   if(f == NULL){
      return NULL;
   }

   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      int flags;
      f->fd = open(filename, flags);
      if(f->fd == -1){
         free(f);
         return NULL;
      }
   #else
      #error Target kernel is not supported
   #endif

   return f;
}
