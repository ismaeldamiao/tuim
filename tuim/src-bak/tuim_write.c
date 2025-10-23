#include <stddef.h>
#include "tuim_impl.h"

void tuim_write(tuim_ctx *ctx, tuim_addr dest, const void *src, tuim_size size){
   if(ctx == NULL) return;
   if(ctx->write(ctx->backend_pointer, dest, src, size) != 0){}
}
