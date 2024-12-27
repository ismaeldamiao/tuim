#include <stddef.h>

size_t strlen(const char* str){
   size_t i;
   for(i = (size_t)0; *str; ++i, ++str);
   return i;
}
