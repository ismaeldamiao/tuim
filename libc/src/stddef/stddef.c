#include <stddef.h>
#include <stdint.h>

#if defined(__linux__) || defined(_WIN32)
   void * const __null = (void*)(uintptr_t)0;
#else
   #error "Unsupported OS."
#endif
