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
   Wrapper for the system calls with six arguments.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
.section .text
   .global WRAPPER
   .type   WRAPPER, STT_FUNC

WRAPPER:
#if   tuim_arch == tuim_arm /* AARCH32 -------------------------------------- */
   PUSH       {r4, r5, lr}
   LDR        r4, [sp, #16]
   LDR        r5, [sp, #20]
   MOV        r12, #SYSTEMCALL
   SVC        #0
   POP        {r4, r5, pc}
#elif tuim_arch == tuim_aarch64 /* AARCH64 ---------------------------------- */
   MOV        x16, SYSTEMCALL
   SVC        #0
   RET
#endif
.size WRAPPER, . - WRAPPER
