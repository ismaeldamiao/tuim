#include <stdint.h>
/* ------------------------------------
   load implementation for the host machine.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */

void tuim_memcpy (uint64_t dest, uint64_t src, uint64_t n); // unit: bytes
void tuim_memcpy2(uint64_t dest, uint64_t src, uint64_t n); // unit: half words
void tuim_memcpy4(uint64_t dest, uint64_t src, uint64_t n); // unit: word
void tuim_memcpy8(uint64_t dest, uint64_t src, uint64_t n); // unit: xwords

void tuim_load(void *dest, uint64_t src, uint64_t n){
   tuim_memcpy((uint64_t)dest, src, n);
}

void tuim_load2(void *dest, uint64_t src, uint64_t n){
   tuim_memcpy2((uint64_t)dest, src, n);
}

void tuim_load4(void *dest, uint64_t src, uint64_t n){
   tuim_memcpy4((uint64_t)dest, src, n);
}

void tuim_load8(void *dest, uint64_t src, uint64_t n){
   tuim_memcpy8((uint64_t)dest, src, n);
}
