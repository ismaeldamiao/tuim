#include <stddef.h>
#include <isa/arm.h>

#define U8(U32) ((\
   (union{ u32 m_big; u8 m_tiny[4]; }){ .m_big = (u32)(U32) }\
).m_tiny)

typedef unsigned _BitInt(8) u8;
typedef unsigned _BitInt(12) u12;
typedef unsigned _BitInt(32) u32;

void* alloc(size_t bytes);

void* crt(void *main, int argc, char **argv, int *ret){
   u32 *crt;
   u8 *argc_ = U8(argc);
   u8 *argv_ = U8(argv);
   u8 *main_ = U8(main);
   u8 *ret_ = U8(ret);
   size_t i;

   crt = alloc((size_t)22 * sizeof(u32));

   // Set the address of main
   crt[i=0] = MOV(R2, main_[0]);
   crt[++i] = ORR(R2, R2, (u12)(0xC00 | main_[1]));
   crt[++i] = ORR(R2, R2, (u12)(0x800 | main_[2]));
   crt[++i] = ORR(R2, R2, (u12)(0x400 | main_[3]));

   // Put argc on the first parameter
   crt[++i] = MOV(R0, argc_[0]);
   crt[++i] = ORR(R0, R0, (u12)(0xC00 | argc_[1]));
   crt[++i] = ORR(R0, R0, (u12)(0x800 | argc_[2]));
   crt[++i] = ORR(R0, R0, (u12)(0x400 | argc_[3]));

   // Put argv on the second parameter
   crt[++i] = MOV(R1, argv_[0]);
   crt[++i] = ORR(R1, R1, (u12)(0xC00 | argv_[1]));
   crt[++i] = ORR(R1, R1, (u12)(0x800 | argv_[2]));
   crt[++i] = ORR(R1, R1, (u12)(0x400 | argv_[3]));

   // Call the main
   crt[++i] = MOV(R10, LR);
   crt[++i] = MOV(LR, PC);
   crt[++i] = BX(R2);

   // Copy the value returned to ret
   crt[++i] = MOV(R2, ret_[0]);
   crt[++i] = ORR(R2, R2, (u12)(0xC00 | ret_[1]));
   crt[++i] = ORR(R2, R2, (u12)(0x800 | ret_[2]));
   crt[++i] = ORR(R2, R2, (u12)(0x400 | ret_[3]));
   crt[++i] = STR(R0, R2);

   // Return to the caller
   crt[++i] = MOV(LR, R10);
   crt[++i] = BX(LR);

   return crt;
}

#include <sys/mman.h>

void* alloc(size_t bytes){
   return mmap(NULL, bytes, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}
