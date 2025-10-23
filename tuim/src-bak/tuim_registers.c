#include "tuim_impl.h"

void tuim_registers(tuim_ctx *ctx, void *new, void *old){
   ctx->registers(ctx->backend_pointer, new, old);
}
