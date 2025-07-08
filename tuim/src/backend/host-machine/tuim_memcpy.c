#include <stddef.h>
#include <stdint.h>
#include "../tuim_backend.h"

#if __STDC_VERSION__ < 199901L
   #define restrict
#endif

void tuim_memcpy(const tuim_ctx *ctx, Elf(Addr) dest, Elf(Addr) src, size_t n){
   register uint8_t * restrict c_dest = (void*)dest;
   register const uint8_t * restrict c_src = (void*)src;
   register const uint8_t * const end_addr = c_src + n;
   (void)ctx;
   while(c_src < end_addr){
      *c_dest = *c_src;
      ++c_dest;
      ++c_src;
   }
}

void tuim_memcpy2(const tuim_ctx *ctx, Elf(Addr) dest, Elf(Addr) src, size_t n){
   register uint16_t * restrict c_dest = (void*)dest;
   register const uint16_t * restrict c_src = (void*)src;
   register const uint16_t * const end_addr = c_src + n;
   (void)ctx;
   while(c_src < end_addr){
      *c_dest = *c_src;
      ++c_dest;
      ++c_src;
   }
}

void tuim_memcpy4(const tuim_ctx *ctx, Elf(Addr) dest, Elf(Addr) src, size_t n){
   register uint32_t * restrict c_dest = (void*)dest;
   register const uint32_t * restrict c_src = (void*)src;
   register const uint32_t * const end_addr = c_src + n;
   (void)ctx;
   while(c_src < end_addr){
      *c_dest = *c_src;
      ++c_dest;
      ++c_src;
   }
}

void tuim_memcpy8(const tuim_ctx *ctx, Elf(Addr) dest, Elf(Addr) src, size_t n){
   register uint64_t * restrict c_dest = (void*)dest;
   register const uint64_t * restrict c_src = (void*)src;
   register const uint64_t * const end_addr = c_src + n;
   (void)ctx;
   while(c_src < end_addr){
      *c_dest = *c_src;
      ++c_dest;
      ++c_src;
   }
}
