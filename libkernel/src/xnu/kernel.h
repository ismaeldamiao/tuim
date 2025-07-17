#ifndef KERNEL_H

#include <stddef.h>

#define tuim_linux      0x0001
#define tuim_xnu        0x0002
#define tuim_windows_nt 0x0003

#define tuim_kernel tuim_linux

typedef long off_t;

void *sys_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int   sys_mprotect(void *addr, size_t size, int prot);
int   sys_munmap(void *addr, size_t length);

int   sys_open(const char *pathname, int flags, .../* mode_t mode */ );
int   sys_write(int fd, const void *buf, size_t len);
off_t sys_lseek(int fildes, off_t offset, int whence);
int   sys_close(int fd);

#endif /* KERNEL_H */
