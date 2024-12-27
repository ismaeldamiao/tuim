#define size_t unsigned int
#define uintptr_t unsigned int

void __aeabi_memcpy8(void *dest, const void *src, size_t n){
   register unsigned long long *c_dest = dest;
   register const unsigned long long *c_src = src;
   register const uintptr_t end_addr = (uintptr_t)dest + n;
   while((uintptr_t)c_dest < end_addr){
      *c_dest = *c_src;
      ++c_dest;
      ++c_src;
   }
}
