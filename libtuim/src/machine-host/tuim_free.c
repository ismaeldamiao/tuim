#include <stdint.h>
#include <stdlib.h>
/* ------------------------------------
   free implementation for the host machine.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */

void tuim_free(uint64_t addr){
   free((void*)(uintptr_t)addr);
}
