#ifndef MACHINE_H
#define MACHINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "elf.h"

/* ------------------------------------
   ...
------------------------------------ */

#if defined(__ARM_ARCH)
   /* Macros defined by ARM Compiler are considered here
      the standard if compiling for ARM target. */
   #if __ARM_ARCH >= 8
      #define ELFCLASS      ELFCLASS64
      #if defined(__ARM_BIG_ENDIAN)
         #define ELFDATA    ELFDATA2MSB
      #else
         #define ELFDATA    ELFDATA2LSB
      #endif
      #define EM_           EM_AARCH64
   #elif __ARM_ARCH <= 7
      #define ELFCLASS      ELFCLASS32
      #if defined(__ARM_BIG_ENDIAN)
         #define ELFDATA    ELFDATA2MSB
      #else
         #define ELFDATA    ELFDATA2LSB
      #endif
      #define EM_           EM_ARM
   #endif
#elif defined(__x86_64__)
   /* FIXME: Macros defined by LLVM CLang are considered here
      the standard when compiling for x86 target. */
   #define ELFCLASS      ELFCLASS64
   #define ELFDATA       ELFDATA2LSB
   #define EM_           EM_X_86_64
#elif defined(__i386__)
   #define ELFCLASS      ELFCLASS32
   #define ELFDATA       ELFDATA2LSB
   #define EM_           EM_386
#elif defined(__riscv)
   /* Macros defined by RISC-V C API are considered
      here the standard when compiling for RISC-V target. */
   #if __riscv_xlen == 64
      #define ELFCLASS      ELFCLASS64
      #define ELFDATA       ELFDATA2LSB // FIXME: Shall be incorrect
      #define EM_           EM_RISCV
   #elif __riscv_xlen == 32
      #define ELFCLASS      ELFCLASS32
      #define ELFDATA       ELFDATA2LSB // FIXME: Shall be incorrect
      #define EM_           EM_RISCV
   #endif
#else
   #error "Architecture not supported."
#endif


#if ELFCLASS == ELFCLASS64
   typedef uint64_t uintN_t;
   typedef int64_t intN_t;
#elif ELFCLASS == ELFCLASS32
   typedef uint32_t uintN_t;
   typedef int32_t intN_t;
#endif // USE_ELF64

#ifdef __cplusplus
}
#endif

#endif // MACHINE_H
