.section .rodata
   .type   new_line, STT_OBJECT
   .size   new_line, 1

new_line:
   .ascii "\n"

.section .text
   .global puts
   .type   puts, STT_FUNC
   .extern strlen

// int puts(const char *s);
.balign 16
puts:
   STR        lr, [sp, #-4]! // push lr on stack
   STR        r7, [sp, #-4]! // push r7 on stack

   // Get the len of the string
   STR        r0, [sp, #-4]! // push the pointer s on stack
   BL         strlen

   // call write(1, s, strlen(s))
   MOV        r2, r0
   LDR        r1, [sp], #4 // pop the pointer s from stack
   MOV        r0, #1
   MOV        r7, #4
   SWI        0x0

   // call write(1, "\n", 1)
   MOV        r2, #1
   LDR        r1, 1f
2: ADD        r1, pc, r1
   MOV        r0, #1
   MOV        r7, #4
   SWI        0x0

   LDR        r7, [sp], #4 // pop r7 from stack
   LDR        pc, [sp], #4 // pop lr from stack
.size	puts, .-puts
1: .4byte     new_line - (2b + 8)
