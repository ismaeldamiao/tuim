#ifndef __SYS_WRITE__
#define __SYS_WRITE__ 1

#include <stddef.h>

#define asm __asm__

#define STATIC static inline

[[gnu::naked]]
STATIC int sys_write(int fd, const void *buf, size_t count){
#if defined(__ARM_ARCH)
#if __ARM_ARCH >= 8

#elif __ARM_ARCH <= 7
   asm("MOV R7, 4");
   asm("SVC 0");
   asm("BX LR");
#endif /* __ARM_ARCH >= 8 */
#else
#error "Unsupported architecture."
#endif /* defined(__ARM_ARCH) */
}

#endif /* __SYS_WRITE__ */
