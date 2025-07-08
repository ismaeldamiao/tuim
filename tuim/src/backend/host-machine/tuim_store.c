#include <stddef.h>
#include "../tuim_backend.h"

void tuim_store(const tuim_ctx *ctx, Elf(Addr) dest, const void *src, size_t n){
   tuim_memcpy(ctx, dest, (Elf(Addr))((void*)src), n);
}

void tuim_store2(const tuim_ctx *ctx, Elf(Addr) dest, const void *src, size_t n){
   tuim_memcpy2(ctx, dest, (Elf(Addr))((void*)src), n);
}

void tuim_store4(const tuim_ctx *ctx, Elf(Addr) dest, const void *src, size_t n){
   tuim_memcpy4(ctx, dest, (Elf(Addr))((void*)src), n);
}

void tuim_store8(const tuim_ctx *ctx, Elf(Addr) dest, const void *src, size_t n){
   tuim_memcpy8(ctx, dest, (Elf(Addr))((void*)src), n);
}
