#include <stdio.h>
#include <stdarg.h>

int fprintf(FILE *stream, const char *format, ...){
  va_list arg;
  int ret;

  va_start(arg, format);
  ret = vfprintf(stream, format, arg);
  va_end(arg);

  return ret;
}
