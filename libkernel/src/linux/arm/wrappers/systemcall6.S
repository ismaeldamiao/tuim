#include "unistd.h"

.section .text
   .global WRAPPER
   .type   WRAPPER, STT_FUNC

WRAPPER:
   PUSH       {r4, r5, r7, lr}
   LDR        r4, [sp, #16]
   LDR        r5, [sp, #20]
   MOV        r7, #SYSTEMCALL
   SVC        #0
   POP        {r4, r5, r7, pc}
.size WRAPPER, . - WRAPPER
