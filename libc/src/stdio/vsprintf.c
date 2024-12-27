#include "printf.h"

int vsprintf(char* s, const char * restrict format, va_list va)
{
  return _vsnprintf(_out_buffer, s, (size_t)-1, format, va);
}
