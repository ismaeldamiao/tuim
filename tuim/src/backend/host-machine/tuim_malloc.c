#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../tuim_backend.h"

Elf_Addr tuim_nullptr = (Elf_Addr)NULL;

Elf_Addr tuim_malloc(const tuim_ctx *ctx, size_t size){
   (void)ctx;
   return (Elf_Addr)malloc(size);
}
