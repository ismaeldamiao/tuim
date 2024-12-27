#include <stddef.h>
#include <stdio.h>
#include <string.h>

#if defined(__linux__)
   #include "linux/sys_write.h"
#else

#endif /* defined(__linux__) */

int putc(int ch, FILE *stream){
   unsigned char ch_;

   if(stream == NULL) return EOF;

   ch_ = (unsigned char)ch;

   /* Call the kernel. */
#if defined(__linux__)
   sys_write(*(int*)stream, &ch_,  (size_t)1);
#else
   #error "Unsupported OS."
#endif /* defined(__linux__) */

   return 0;
}
