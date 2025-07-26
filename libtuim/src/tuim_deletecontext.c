#include "tuim_impl.h"
/* ------------------------------------
   Function to create a new intance of tuim_ctx.
   * Part of Tuim Project.
   * Last modified: June 17, 2025.
------------------------------------ */

void free(void*);

void tuim_deletecontext(tuim_ctx *ctx){
   ctx->delete_vm(ctx->backend_pointer);
   free(ctx);
}
