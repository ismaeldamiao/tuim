#define DEBUG 0
#if DEBUG
   int printf(const char *, ...);
   int puts(const char *);
#endif

/* sanity checks */

#ifndef __STDC__
   #error the C compiler shall be standard compliant
#endif

#if !defined(__unix__) && !(__APPLE__ && __MACH__) && !defined(_WIN32)
   #error the host OS is not supported
#endif

/* language extensions */

#if __STDC_VERSION__ < 202311L
   #define true 1
   #define false 0
#endif
#if __STDC_VERSION__ < 199901L
   #define bool int
#elif (__STDC_VERSION__ >= 199901L) && (__STDC_VERSION__ < 202311L)
   typedef _Bool bool;
#endif

#if __STDC_VERSION__ < 201112L
   #define thread_local
#elif  (__STDC_VERSION__ >= 201112L) && (__STDC_VERSION__ < 202311L)
   #define thread_local _Thread_local
#endif

#if __STDC_VERSION__ < 201112L
   #define alignof(x)
#elif  (__STDC_VERSION__ >= 201112L) && (__STDC_VERSION__ < 202311L)
   #define alignas(x) _Alignas(x)
#endif

#if __STDC_VERSION__ < 199901L
   #define restrict
#endif

/* Swap macros */

#define swap16(x) (x)
#define swap32(x) (x)
#define swap64(x) (x)

/* TUIM_BUILD_FLAGS */

#define TUIM_BF_ELF32 (1 << 0)
#define TUIM_BF_ELF64 (1 << 1)

#if defined(__unix__) || (__APPLE__ && __MACH__)
   #define _POSIX_C_SOURCE 200112L
#endif

#if /* supported 32 platforms */ \
   (defined(__riscv) && __riscv == 32) || \
   (defined(__ARM_ARCH) && __ARM_ARCH <= 7) || \
   defined(__i386__)
      #define TUIM_BUILD_FLAGS TUIM_BF_ELF32
#elif /* supported 64 platforms (with ELF32 and ELF64 support) */ \
   (defined(__riscv) && __riscv == 64) || \
   (defined(__ARM_ARCH) && __ARM_ARCH >= 8) || \
   defined(__x86_64__)
      #define TUIM_BUILD_FLAGS (TUIM_BF_ELF32 | TUIM_BF_ELF64)
#endif
