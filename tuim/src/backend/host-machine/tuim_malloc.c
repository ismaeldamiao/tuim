#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../tuim_backend.h"

Elf(Addr) tuim_nullptr = (Elf(Addr))NULL;

Elf(Addr) tuim_malloc(const tuim_ctx *ctx, size_t size){
   (void)ctx;
   return (Elf(Addr))malloc(size);
}
