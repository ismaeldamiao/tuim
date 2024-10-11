// clang -c -fPIC --target=arm-unknown-none -I../include -o ola_mundo.o ola_mundo.c
// ld -pie -e main -o ola_mundo ola_mundo.o
#include <stdint.h>
#include <isa/arm.h>

static const char msg_buf[] = "Ola Mundo!\n";
static const uint32_t msg_len = (uint32_t)sizeof(msg_buf);

[[gnu::section(".text"), gnu::aligned(4)]]
const uint32_t putc[] = {
   // int write(int fd, const void *buf, unsigned int count);
   MOV(R7, (unsigned _BitInt(8))4), // arg0 <- SYS_WRITE
   MOV(R0, (unsigned _BitInt(8))1), // arg1 <- stdout
   LDR(R1, PC, 8), // arg2 <- message
   LDR(R2, PC, 8), // arg2 <- len
   SVC(0), // Software interrupt: Linux system call

   BX(LR), // Return to the caller
   (uint32_t)msg_buf,
   msg_len,
};
