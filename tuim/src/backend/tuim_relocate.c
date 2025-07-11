#if defined(TUIM_BUILD_RISCV32)
   #error Not implemented yet
#elif defined(TUIM_BUILD_RISCV64)
   #error Not implemented yet
#elif defined(TUIM_BUILD_AARCH32)
   #include "armel/tuim_relocate.c"
#elif defined(TUIM_BUILD_AARCH64)
   #include "aarch64/tuim_relocate.c"
#elif defined(TUIM_BUILD_AMD64)
   #include "amd64/tuim_relocate.c"
#endif
