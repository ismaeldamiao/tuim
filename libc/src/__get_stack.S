#include <tuim/arch.h>

.section .text
   .global __get_stack
   .type   __get_stack, STT_FUNC

__get_stack:
#if   tuim_arch == tuim_riscv64 || \
      tuim_arch == tuim_riscv32 /* RISC-V ----------------------------------- */

#elif tuim_arch == tuim_arm /* AARCH32 -------------------------------------- */
   MOV        r0, sp
   BX         lr
#elif tuim_arch == tuim_aarch64 /* AARCH64 ---------------------------------- */

#elif tuim_arch == tuim_i386 /* i386 ---------------------------------------- */

#elif tuim_arch == tuim_amd64 /* AMD64 -------------------------------------- */

#endif
.size __get_stack, . - __get_stack
