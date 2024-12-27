#include <stdio.h>

int puts(const char *str){
   if(fputs(str, stdout) < 0) return EOF;
   if(fputc((int)'\n', stdout) < 0) return EOF;
   return 0;
}
