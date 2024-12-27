#include <stddef.h>
#include <stdio.h>
#include <string.h>

#if defined(__linux__)
   #include "linux/sys_write.h"
#else

#endif /* defined(__linux__) */

int fputs(const char *s, FILE *stream){
   size_t s_len;

   if(stream == NULL) return EOF;

   s_len = strlen(s);

   /* Call the kernel. */
#if defined(__linux__)
   sys_write(*(int*)stream, s,  s_len);
#else
   #error "Unsupported OS."
#endif /* defined(__linux__) */

   return 0;
}
