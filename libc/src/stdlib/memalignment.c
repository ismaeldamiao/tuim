#include <stddef.h>

size_t memalignment(const void *p){
   return (size_t)p & - (size_t)p;
}
