#if defined(__riscv) /* RISC-V ---------------------------------------------- */
#define __NR_write 64



#elif defined(__ARM_ARCH) && (__ARM_ARCH <= 7) /* AARCH32 ------------------- */
#define __NR_write 4



#elif defined(__ARM_ARCH) && (__ARM_ARCH >= 8) /* AARCH64 ------------------- */
#define __NR_write 64



#elif defined(__i386__) /* i386 --------------------------------------------- */
#define __NR_write 4



#elif defined(__x86_64__) /* AMD64 ------------------------------------------ */
#define __NR_write 64



#endif /* defined(__riscv) */
