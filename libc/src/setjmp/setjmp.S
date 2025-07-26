#include <tuim/arch.h>

.section .text
   .global __setjmp
   .type   __setjmp, STT_FUNC
   .global setjmp
   .type   setjmp, STT_FUNC

__setjmp:
setjmp:
#if   tuim_arch == tuim_riscv64 || \
      tuim_arch == tuim_riscv32 /* RISC-V ----------------------------------- */

#elif tuim_arch == tuim_arm /* AARCH32 -------------------------------------- */
   MOV        ip,  r0
   STMIA      ip!, {v1,v2,v3,v4,v5,v6,sl,fp}
   MOV        r2,  sp
   STMIA      ip!, {r2,lr}
   MOV        r0,  #0

   ADR        r1,  1f
   LDR        r2,  1f
   LDR        r1,  [r1,r2]

   TST        r1,  #0x260
   BEQ        3f
   // HWCAP_ARM_FPA
   TST        r1,  #0x20
   BEQ        2f
   STC        p2,  cr4, [ip], #48

2: TST        r1,  #0x40
   BEQ        2f
   .FPU       vfp
   VSTMIA     ip!, {d8-d15}
   .FPU       softvfp
   .eabi_attribute 10, 0
   .eabi_attribute 27, 0

   // HWCAP_ARM_IWMMXT
2: TST r1,#0x200
   BEQ 3f
   STCL       p1, cr10, [ip], #8
   STCL       p1, cr11, [ip], #8
   STCL       p1, cr12, [ip], #8
   STCL       p1, cr13, [ip], #8
   STCL       p1, cr14, [ip], #8
   STCL       p1, cr15, [ip], #8
2:
3: BX         lr

.hidden __hwcap
.align 2
1:	.word __hwcap-1b

#if 1
__hwcap: MOV r0, r1
#endif

#elif tuim_arch == tuim_aarch64 /* AARCH64 ---------------------------------- */

#elif tuim_arch == tuim_i386 /* i386 ---------------------------------------- */

#elif tuim_arch == tuim_amd64 /* AMD64 -------------------------------------- */

#endif
.size setjmp, . - setjmp
.size __setjmp, . - setjmp
