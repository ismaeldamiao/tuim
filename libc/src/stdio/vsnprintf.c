#include "printf.h"

int vsnprintf(char* s, size_t n, const char * restrict format, va_list va)
{
  return _vsnprintf(_out_buffer, s, n, format, va);
}
