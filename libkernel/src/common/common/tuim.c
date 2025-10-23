#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

#include <string.h>

#include <tuim.h>
#include <tuim/kernel.h>

typedef int (*tuim_f)(int routine, va_list args);
struct tuim_ctx {
   tuim_f tuim_va;
};

extern int tuim_va(int routine, va_list args);

int tuim(int routine, ...){
   va_list args, args_copy;
   struct tuim_ctx *ctx;

   char *tuim_home, *ld_library_path, *machine_so;
   int status;

   va_start(args, routine);

   /* if not creating a new execution context, call backend */
   if(routine != TUIM_NEWCONTEXT){
      va_copy(args_copy, args);
      ctx = va_arg(args_copy, struct tuim_ctx *);
      va_end(args_copy);

      return ctx->tuim_va(routine, args);
   }

   va_copy(args_copy, args);
   tuim_home = va_arg(args_copy, char *);
   ld_library_path = va_arg(args_copy, char *);
   machine_so = va_arg(args_copy, char *);
   va_end(args_copy);

   if(machine_so == NULL){
      /* call native backend */
      return tuim_va(routine, args);
   }else{
      char so_path[512];
      uintptr_t address;
      tuim_f tuim_va;

      if(machine_so[0] == '/'){
         strcpy(so_path, tuim_home);
         strcat(so_path, machine_so);
         machine_so = so_path;
      }
      status = tuim(TUIM_LOAD, tuim_thiscontext, machine_so);
      if(status != TUIM_SUCCESS) return status;
      status = tuim(TUIM_SYMBOL, tuim_thiscontext, "tuim_va", &address);
      if(status != TUIM_SUCCESS) return status;
      tuim_va = (tuim_f)(void*)address;

      /* call backend */
      return tuim_va(routine, args);
   }
}
