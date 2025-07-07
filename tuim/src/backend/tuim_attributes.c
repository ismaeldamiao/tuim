#if defined(__ARM_ARCH)
   #if __ARM_ARCH < 7
      #include "armel/tuim_attributes.c"
   #elif __ARM_ARCH == 7
      #include "armv7l/tuim_attributes.c"
   #elif __ARM_ARCH > 7
      #include "aarch64/tuim_attributes.c"
   #endif
#else
   #error Unknown target architecture
#endif
