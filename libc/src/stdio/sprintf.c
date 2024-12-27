#include <stdio.h>
#include <stdarg.h>

int sprintf(char * restrict buffer, const char * restrict format, ...){
  va_list arg;
  int ret;

  va_start(arg, format);
  ret = vsprintf(buffer, format, arg);
  va_end(arg);

  return ret;
}
