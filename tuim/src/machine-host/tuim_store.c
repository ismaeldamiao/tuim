#include <stdint.h>
/* ------------------------------------
   store implementation for the host machine.
   * Part of Tuim Project.
   * Last modified: July 11, 2025.
------------------------------------ */

void tuim_memcpy (uint64_t dest, uint64_t src, uint64_t n); // unit: bytes
void tuim_memcpy2(uint64_t dest, uint64_t src, uint64_t n); // unit: half words
void tuim_memcpy4(uint64_t dest, uint64_t src, uint64_t n); // unit: word
void tuim_memcpy8(uint64_t dest, uint64_t src, uint64_t n); // unit: xwords

void tuim_store(uint64_t dest, const void *src, uint64_t n){
   tuim_memcpy(dest, (uint64_t)((void*)src), n);
}

void tuim_store2(uint64_t dest, const void *src, uint64_t n){
   tuim_memcpy2(dest, (uint64_t)((void*)src), n);
}

void tuim_store4(uint64_t dest, const void *src, uint64_t n){
   tuim_memcpy4(dest, (uint64_t)((void*)src), n);
}

void tuim_store8(uint64_t dest, const void *src, uint64_t n){
   tuim_memcpy8(dest, (uint64_t)((void*)src), n);
}
