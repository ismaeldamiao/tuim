#include "tuim.h"
#include "tuim_ctx.h"

int tuim_errno(tuim_ctx *ctx){
   return ctx->status;
}
