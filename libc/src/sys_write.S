/* *****************************************************************************
   MIT License

   Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the “Software”), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
***************************************************************************** */
#include "unistd.h"
/* ------------------------------------
   Wrapper for the sys_write system call.
   * Part of Tuim Project.
   * Last modified: February 28, 2025.
------------------------------------ */
.section .text
   .global _sys_write
   .type   _sys_write, STT_FUNC

_sys_write:
#if defined(__riscv) /* RISC-V ---------------------------------------------- */
   LI         a7, __NR_write
   ECALL
   JR         ra
#elif defined(__ARM_ARCH) && (__ARM_ARCH <= 7) /* AARCH32 ------------------- */
   PUSH       {r7, lr}
   MOV        r7, #__NR_write
   SVC        #0
   POP        {r7, lr}
   BX         lr
#elif defined(__ARM_ARCH) && (__ARM_ARCH >= 8) /* AARCH64 ------------------- */
   MOV        x8, __NR_write
   SVC        #0
   RET
#elif defined(__i386__) /* i386 --------------------------------------------- */
   PUSH        %ebp
   MOV         %ebp, %esp
   MOV         %eax, __NR_write
   INT         $0x80
   MOV         %esp, %ebp
   POP         %ebp
   RET
#elif defined(__x86_64__) /* AMD64 ------------------------------------------ */
   MOV        %rax, __NR_write
   SYSCALL
   RET
#endif /* defined(__riscv) */
.size _sys_write, . - _sys_write
