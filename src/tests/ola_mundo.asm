// as -k -o ola_mundo.o ola_mundo.asm
// ld -pie -e main -o ola_mundo ola_mundo.o
// ./ola_mundo

.data
message: .ascii "Ola Mundo!\n" // char[]
len:     .4byte . - message    // int32_t len = sizeof(message)

.text
.global main
main:
   // int write(int fd, const void *buf, unsigned int count);
   mov r7, #4       // arg0 <- SYS_WRITE
   mov r0, #1       // arg1 <- stdout
   ldr r1, [pc, #8] // arg2 <- message
   ldr r2, [pc, #8] // arg3 <- len
   svc #0           // Software interrupt: Linux system call
   //mov r0, r2

   bx lr // Return to the caller

   .4byte message
   .4byte len
