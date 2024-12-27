#include <stdio.h>
#include <stdarg.h>

int printf(const char *format, ...){
  va_list arg;
  int ret;

  va_start(arg, format);
  ret = vprintf( format, arg);
  va_end(arg);

  return ret;
}
