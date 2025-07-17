#include <stdint.h>
/* ------------------------------------
   memcpy implementation for the host machine.
   * Part of Tuim Project.
   * Last modified: July 11, 2025.
------------------------------------ */

void tuim_memcpy(uint64_t dest, uint64_t src, uint64_t n){
   register uint8_t * restrict c_dest = (void*)(uintptr_t)dest;
   register const uint8_t * restrict c_src = (void*)(uintptr_t)src;
   register const uint8_t * const end_addr = c_src + n;

   while(c_src < end_addr){
      *c_dest = *c_src;
      ++c_dest;
      ++c_src;
   }
}

void tuim_memcpy2(uint64_t dest, uint64_t src, uint64_t n){
   register uint16_t * restrict c_dest = (void*)(uintptr_t)dest;
   register const uint16_t * restrict c_src = (void*)(uintptr_t)src;
   register const uint16_t * const end_addr = c_src + n;

   while(c_src < end_addr){
      *c_dest = *c_src;
      ++c_dest;
      ++c_src;
   }
}

void tuim_memcpy4(uint64_t dest, uint64_t src, uint64_t n){
   register uint32_t * restrict c_dest = (void*)(uintptr_t)dest;
   register const uint32_t * restrict c_src = (void*)(uintptr_t)src;
   register const uint32_t * const end_addr = c_src + n;

   while(c_src < end_addr){
      *c_dest = *c_src;
      ++c_dest;
      ++c_src;
   }
}

void tuim_memcpy8(uint64_t dest, uint64_t src, uint64_t n){
   register uint64_t * restrict c_dest = (void*)(uintptr_t)dest;
   register const uint64_t * restrict c_src = (void*)(uintptr_t)src;
   register const uint64_t * const end_addr = c_src + n;

   while(c_src < end_addr){
      *c_dest = *c_src;
      ++c_dest;
      ++c_src;
   }
}
