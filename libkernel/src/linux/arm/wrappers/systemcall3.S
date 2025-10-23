#include "unistd.h"

.section .text
   .global WRAPPER
   .type   WRAPPER, STT_FUNC

WRAPPER:
   PUSH       {r7, lr}
   MOV        r7, #SYSTEMCALL
   SVC        #0
   POP        {r7, pc}
.size WRAPPER, . - WRAPPER
