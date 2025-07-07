#if defined(__ARM_ARCH)
   #if __ARM_ARCH <= 7
      #include "armel/tuim_relocate.c"
   #elif __ARM_ARCH > 7
      #include "aarch64/tuim_rellocate.c"
   #endif
#else
   #error Unknown target architecture
#endif
