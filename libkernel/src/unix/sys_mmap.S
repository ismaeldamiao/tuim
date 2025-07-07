#include "unistd.h"
/* ------------------------------------
   Wrapper for the sys_mmap system call.
   * Part of Tuim Project.
   * Last modified: June 24, 2025.
------------------------------------ */
.section .text
   .global _sys_mmap
   .type   _sys_mmap, STT_FUNC

_sys_mmap:
#if defined(__riscv) /* RISC-V ---------------------------------------------- */
   LI         a7, __NR_mmap
   ECALL
   JR         ra

#elif defined(__ARM_ARCH) && (__ARM_ARCH <= 7) /* AARCH32 ------------------- */
   PUSH       {r7, lr}
   MOV        r7, #__NR_mmap
   SVC        #0
   POP        {r7, lr}
   BX         lr

#elif defined(__ARM_ARCH) && (__ARM_ARCH >= 8) /* AARCH64 ------------------- */
   MOV        x8, __NR_mmap
   SVC        #0
   RET

#elif defined(__i386__) /* i386 --------------------------------------------- */
   PUSH       %ebp
   MOV        %ebp, %esp
   MOV        %eax, __NR_mmap
   INT        $0x80
   MOV        %esp, %ebp
   POP        %ebp
   RET

#elif defined(__x86_64__) /* AMD64 ------------------------------------------ */
   MOV        %rax, __NR_mmap
   SYSCALL
   RET

#endif /* defined(__riscv) */
.size _sys_mmap, . - _sys_mmap
