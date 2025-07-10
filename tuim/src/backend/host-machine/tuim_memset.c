#include <stddef.h>
#include <stdint.h>
#include "../tuim_backend.h"

void tuim_memset(const tuim_ctx *ctx, Elf_Addr dest, int c, size_t n){
   register uint8_t *c_dest = (void*)dest;
   register const uint8_t ch = (uint8_t)c;
   register const uint8_t * end_addr = (void*)(dest + n);
   (void)ctx;
   while(c_dest < end_addr){
      *c_dest = ch;
      ++c_dest;
   }
}

void tuim_memset2(const tuim_ctx *ctx, Elf_Addr dest, int c, size_t n){
   register uint16_t *c_dest = (void*)dest;
   register const uint16_t ch = (uint16_t)c;
   register const uint16_t * end_addr = (void*)(dest + 2*n);
   (void)ctx;
   while(c_dest < end_addr){
      *c_dest = ch;
      ++c_dest;
   }
}

void tuim_memset4(const tuim_ctx *ctx, Elf_Addr dest, int c, size_t n){
   register uint32_t *c_dest = (void*)dest;
   register const uint32_t ch = (uint32_t)c;
   register const uint32_t * end_addr = (void*)(dest + 4*n);
   (void)ctx;
   while(c_dest < end_addr){
      *c_dest = ch;
      ++c_dest;
   }
}

void tuim_memset8(const tuim_ctx *ctx, Elf_Addr dest, int c, size_t n){
   register uint64_t *c_dest = (void*)dest;
   register const uint64_t ch = (uint64_t)c;
   register const uint64_t * end_addr = (void*)(dest + 8*n);
   (void)ctx;
   while(c_dest < end_addr){
      *c_dest = ch;
      ++c_dest;
   }
}
