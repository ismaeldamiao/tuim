#include <stdio.h>
#include <stdarg.h>

int snprintf(char * restrict s, size_t n, const char * restrict format, ...){
  va_list arg;
  int ret;

  va_start(arg, format);
  ret = vsnprintf(s, n, format, arg);
  va_end(arg);

  return ret;
}
