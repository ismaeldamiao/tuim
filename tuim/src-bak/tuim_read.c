#include <stddef.h>
#include "tuim_impl.h"

void tuim_read(tuim_ctx *ctx, void *dest, tuim_addr src, tuim_size size){
   if(ctx == NULL) return;
   if(ctx->read(ctx->backend_pointer, dest, src, size) != 0){}
}
