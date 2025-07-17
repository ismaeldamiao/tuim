#include <stddef.h>
#include <stdlib.h>

int posix_memalign(void **memptr, size_t alignment, size_t size);

void *aligned_alloc(size_t alignment, size_t size){
   void *ptr;
   if(posix_memalign(&ptr, alignment, size) != 0) return NULL;
   return ptr;
}
