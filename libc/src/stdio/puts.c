#include <stddef.h>
#include <string.h>

#include <kernel.h>

int puts(const char *s){
   #ifdef __unix__
      sys_write(1, (void*)s, strlen(s));
      sys_write(1, "\n", (size_t)1U);
   #else
      #error Target kernel is not supported
   #endif
   return 0;
}
