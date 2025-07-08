#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../tuim_backend.h"

void tuim_free(const tuim_ctx *ctx, Elf(Addr) addr){
   (void)ctx;
   free((void*)addr);
}
