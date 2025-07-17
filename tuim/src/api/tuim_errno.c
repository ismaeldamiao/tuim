#include "tuim_impl.h"

int tuim_errno(tuim_ctx *ctx){
   return ctx->status;
}
