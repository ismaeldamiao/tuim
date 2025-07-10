#include <stddef.h>
#include "../tuim_backend.h"

void tuim_load(const tuim_ctx *ctx, void *dest, Elf_Addr src, size_t n){
   tuim_memcpy(ctx, (Elf_Addr)dest, src, n);
}

void tuim_load2(const tuim_ctx *ctx, void *dest, Elf_Addr src, size_t n){
   tuim_memcpy2(ctx, (Elf_Addr)dest, src, n);
}

void tuim_load4(const tuim_ctx *ctx, void *dest, Elf_Addr src, size_t n){
   tuim_memcpy4(ctx, (Elf_Addr)dest, src, n);
}

void tuim_load8(const tuim_ctx *ctx, void *dest, Elf_Addr src, size_t n){
   tuim_memcpy8(ctx, (Elf_Addr)dest, src, n);
}
