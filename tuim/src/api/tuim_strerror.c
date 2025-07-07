#include "tuim.h"
#include "tuim_ctx.h"

#include "ascii/ascii.h"

char *tuim_strerror(tuim_ctx *ctx){
   return string(ctx->strerror);
}
