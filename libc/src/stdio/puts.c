#include <stddef.h>
#include <string.h>

#include <tuim/kernel.h>

int puts(const char *s){
   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      write(1, s, strlen(s));
      write(1, "\n", (size_t)1U);
   #else
      #error Target kernel is not supported
   #endif
   return 0;
}
