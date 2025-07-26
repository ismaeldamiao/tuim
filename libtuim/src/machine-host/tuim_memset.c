#include <stdint.h>
/* ------------------------------------
   memset implementation for the host machine.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */

void tuim_memset(uint64_t dest, int c, uint64_t n){
   register uint8_t *c_dest = (void*)(uintptr_t)dest;
   register const uint8_t ch = (uint8_t)c;
   register const uint8_t * end_addr = (void*)(uintptr_t)(dest + n);

   while(c_dest < end_addr){
      *c_dest = ch;
      ++c_dest;
   }
}

void tuim_memset2(uint64_t dest, int c, uint64_t n){
   register uint16_t *c_dest = (void*)(uintptr_t)dest;
   register const uint16_t ch = (uint16_t)c;
   register const uint16_t * end_addr = (void*)(uintptr_t)(dest + 2*n);

   while(c_dest < end_addr){
      *c_dest = ch;
      ++c_dest;
   }
}

void tuim_memset4(uint64_t dest, int c, uint64_t n){
   register uint32_t *c_dest = (void*)(uintptr_t)dest;
   register const uint32_t ch = (uint32_t)c;
   register const uint32_t * end_addr = (void*)(uintptr_t)(dest + 4*n);

   while(c_dest < end_addr){
      *c_dest = ch;
      ++c_dest;
   }
}

void tuim_memset8(uint64_t dest, int c, uint64_t n){
   register uint64_t *c_dest = (void*)(uintptr_t)dest;
   register const uint64_t ch = (uint64_t)c;
   register const uint64_t * end_addr = (void*)(uintptr_t)(dest + 8*n);

   while(c_dest < end_addr){
      *c_dest = ch;
      ++c_dest;
   }
}
