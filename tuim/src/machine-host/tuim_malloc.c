#include <stdint.h>
#include <stdlib.h>
/* ------------------------------------
   malloc implementation for the host machine.
   * Part of Tuim Project.
   * Last modified: July 16, 2025.
------------------------------------ */

const uint64_t tuim_nullptr = (uint64_t)(uintptr_t)NULL;

uint64_t tuim_malloc(uint64_t size){
   return (uint64_t)malloc((size_t)size);
}
