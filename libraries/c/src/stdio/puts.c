#include <stddef.h>
#include <string.h>

int _sys_write(int fd, void *buf, size_t len);

int puts(const char *s){
   _sys_write(1, (void*)s, strlen(s));
   _sys_write(1, "\n", (size_t)1U);
   return 0;
}
