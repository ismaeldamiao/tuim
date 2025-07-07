#include <stddef.h>
#include <string.h>

#ifdef __unix__
   int _sys_write(int fd, void *buf, size_t len);
#endif

int puts(const char *s){
   #ifdef __unix__
      _sys_write(1, (void*)s, strlen(s));
      _sys_write(1, "\n", (size_t)1U);
   #else
      #error Target kernel is not supported
   #endif
   return 0;
}
