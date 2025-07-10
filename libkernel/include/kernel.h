#ifndef __KERNEL_H

#include <stddef.h>

#if defined(__unix__)
   int sys_write(int fd, void *buf, size_t len);
#endif

#endif /* __KERNEL_H */
