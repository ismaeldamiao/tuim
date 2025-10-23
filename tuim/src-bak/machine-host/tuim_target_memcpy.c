#include <tuim.h>

int tuim_target_memcpy(void *ptr, tuim_addr dest, tuim_addr src, tuim_size size){
   /* TODO: I'm lazy and there is no much code using COPY relocation */
   (void)ptr;
   (void)dest;
   (void)src;
   (void)size;
   return 1;
}
