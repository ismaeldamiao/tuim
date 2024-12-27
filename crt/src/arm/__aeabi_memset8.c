#define size_t unsigned int
#define uintptr_t unsigned int

void __aeabi_memset8(void *dest, size_t n, int c){
   register unsigned long long *c_dest = dest;
   register const unsigned int ch = (typeof(ch))c;
   register const uintptr_t end_addr = (uintptr_t)dest + n;
   while((uintptr_t)c_dest < end_addr){
      *c_dest = ch;
      ++c_dest;
   }
}
