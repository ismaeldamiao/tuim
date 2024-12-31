#include <stddef.h>
#include <string.h>

bool usetmp = false;



void tdk_GetDefaultTarget(char *default_target){
   /* Default target */
   #if defined(__ARM_ARCH)
      #if defined(__ARM_ARCH_7A__)
         strcpy(default_target, "armv7a");
      #else
         strcpy(default_target, "arm");
      #endif // __ARM_ARCH == 7
   #else
      default_target[0] = '\0';
   #endif // defined(__ARM_ARCH)
}
