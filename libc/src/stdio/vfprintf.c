#include "printf.h"

int vfprintf(FILE * restrict stream, const char * restrict format, va_list va)
{
  return _vsnprintf(_out_stream, (void*)stream, (size_t)-1, format, va);
}
