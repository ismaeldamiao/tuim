/* *****************************************************************************
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
#include <tuim/arch.h>
#include "unistd.h"
/* ------------------------------------
   Wrapper for the system calls with four arguments.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
.section .text
   .global WRAPPER
   .type   WRAPPER, STT_FUNC

WRAPPER:
#if   tuim_arch == tuim_riscv64 || \
      tuim_arch == tuim_riscv32 /* RISC-V ----------------------------------- */
   LI         a7, SYSTEMCALL
   ECALL
   JR         ra
#elif tuim_arch == tuim_arm /* AARCH32 -------------------------------------- */
   PUSH       {r7, lr}
   MOV        r7, #SYSTEMCALL
   SVC        #0
   POP        {r7, pc}
#elif tuim_arch == tuim_aarch64 /* AARCH64 ---------------------------------- */
   MOV        x8, SYSTEMCALL
   SVC        #0
   RET
#elif tuim_arch == tuim_i386 /* i386 ---------------------------------------- */
   PUSH        %ebp
   MOV         %ebp, %esp
   MOV         %eax, SYSTEMCALL
   INT         $0x80
   MOV         %esp, %ebp
   POP         %ebp
   RET
#elif tuim_arch == tuim_amd64 /* AMD64 -------------------------------------- */
   MOV        %rax, SYSTEMCALL
   SYSCALL
   RET
#endif
.size WRAPPER, . - WRAPPER
