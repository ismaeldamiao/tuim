#include <tuim/arch.h>

.section .data
   .global __libc_environment
   .type   __libc_environment, STT_OBJECT

__libc_environment:
#if   tuim_arch == tuim_riscv32 || \
      tuim_arch == tuim_arm || \
      tuim_arch == tuim_i386
   .word 0x00000000
#elif tuim_arch == tuim_riscv64 || \
      tuim_arch == tuim_aarch64 || \
      tuim_arch == tuim_amd64
   .word 0x00000000
   .word 0x00000000
#endif
.size __libc_environment, . - __libc_environment

.section .text
   .global __libc_main
   .type   __libc_main, STT_FUNC

__libc_main:
#if   tuim_arch == tuim_riscv64 || \
      tuim_arch == tuim_riscv32 /* RISC-V ----------------------------------- */

#elif tuim_arch == tuim_arm /* AARCH32 -------------------------------------- */
   MOV         r3, r0            // save the address of main on r3
   LDR         r0, [sp]          // r0 <- argc
   ADD         r1, sp, #4        // r1 <- argv

   STR         r4, [sp, #-0x4]!  // PUSH {r4}
   MOV         r2, #4
   ADD         pc, pc, r2
   .word       __libc_environment(GOT)
   .word       _GLOBAL_OFFSET_TABLE_ - . - 0x14
   LDR         r2, [pc, #-0x10]
   LDR         r4, [pc, #-0x10]
   ADD         r4, r4, pc
   ADD         r4, r4, r2
   LDR         r4, [r4]          // r4 <- __libc_environment
   MOV         r2, #4
   MUL         r2, r0, r2
   ADD         r2, r1, r2
   ADD         r2, r2, #4        // r2 <- envp
   STR         r2, [r4]          // __libc_environment <- envp
   LDR         r4, [sp], #4      // POP {r4}

   STR         lr, [sp, #-0x4]!  // PUSH {lr}
   MOV         lr, pc
   MOV         pc, r3            // call main
   LDR         r1, [sp]          // POP {lr}
   STR         r0, [sp]          // return on stack
   MOV         pc, r1
#elif tuim_arch == tuim_aarch64 /* AARCH64 ---------------------------------- */

#elif tuim_arch == tuim_i386 /* i386 ---------------------------------------- */

#elif tuim_arch == tuim_amd64 /* AMD64 -------------------------------------- */

#endif
.size __libc_main, . - __libc_main
