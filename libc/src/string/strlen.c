#include <stddef.h>

size_t strlen(const char *s){
   const char * const s_cpy = s;
   while(*s != '\0') ++s;
   return (size_t)(s - s_cpy);
}
