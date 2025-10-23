#include <stdint.h>
#include <tuim.h>

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 202311L
#if __has_include(<tuim/kernel.h>)
   #define tuim_environment
#endif // __has_include(<tuim/kernel.h>)
#endif /* __STDC_VERSION__ < 202311L */
#endif /* defined(__STDC_VERSION__) */

#if defined(tuim_environment)
   #include <tuim/arch.h>
   #include <tuim/kernel.h>
#else
   #include "arch.h"
   #include "kernel.h"
   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      #include <stdio.h>
      #include <fcntl.h>
      #include <unistd.h>
   #endif
#endif

int tuim_target_write(void *ptr, tuim_addr dest, const void *src, tuim_size size){
   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      /* On these system I need to read the file /proc/pid/mem
         and then read the memory from that. */
      pid_t pid;
      int fd;
      char file_name[0x12];
      union aliasing { off_t offset; uintptr_t address; } aliasing;

      pid = *(pid_t*)ptr;
      sprintf(file_name, "/proc/%d/mem", (int)pid);
      fd = open(file_name, O_WRONLY);
      if(fd == -1){
         return 1;
      }

      aliasing.address = (uintptr_t)dest;
      if(lseek(fd, aliasing.offset, SEEK_SET) == (off_t)(-1)){
         close(fd);
         return 1;
      }

      if(write(fd, src, (size_t)size) == (ssize_t)(-1)){
         close(fd);
         return TUIM_EFAULT;
      }
      close(fd);

      return TUIM_SUCCESS;
   #endif
}
