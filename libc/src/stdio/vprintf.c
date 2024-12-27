#include "printf.h"

int vprintf(const char * restrict format, va_list va){
   char s[1];
  return _vsnprintf(_out_char, s, (size_t)-1, format, va);
}
