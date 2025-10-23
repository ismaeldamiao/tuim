#ifndef KERNEL_H

#include <stddef.h>

#define tuim_unknown          0x0000
// from 0x0001 to 0x000f, inclusive, are reserved for future use
#define tuim_linux            0x0010
#define tuim_xnu              0x0011
#define tuim_windows_nt       0x0012

#define tuim_kernel           tuim_linux

/* --- */
/* FIXME: replace unknowns */

#define O_RDONLY     0
#define O_WRONLY     1
#define O_RDWR       2

#define O_APPEND     0 /* unknown */
#define O_CREAT      0 /* unknown */
#define O_DSYNC      0 /* unknown */
#define O_EXCL       0 /* unknown */
#define O_NOCTTY     0 /* unknown */
#define O_NONBLOCK   0 /* unknown */
#define O_RSYNC      0 /* unknown */
#define O_SYNC       0 /* unknown */
#define O_TRUNC      0 /* unknown */

#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2

#define PROT_READ    0 /* unknown */
#define PROT_WRITE   0 /* unknown */
#define PROT_EXEC    0 /* unknown */
#define PROT_NONE    0 /* unknown */

#define MAP_ANON        0 /* unknown */
#define MAP_ANONYMOUS   0 /* unknown */
#define MAP_SHARED      0 /* unknown */
#define MAP_PRIVATE     0 /* unknown */
#define MAP_FIXED       0 /* unknown */
#define MAP_FAILED      0 /* unknown */

#define PTRACE_TRACEME     0 /* unknown */
#define PTRACE_PEEKTEXT    0 /* unknown */
#define PTRACE_PEEKDATA    0 /* unknown */
#define PTRACE_PEEKUSER    0 /* unknown */
#define PTRACE_POKETEXT    0 /* unknown */
#define PTRACE_POKEDATA    0 /* unknown */
#define PTRACE_POKEUSER    0 /* unknown */
#define PTRACE_GETREGS     0 /* unknown */
#define PTRACE_GETFPREGS   0 /* unknown */
#define PTRACE_GETREGSET   0 /* unknown */
#define PTRACE_SETREGS     0 /* unknown */
#define PTRACE_SETFPREGS   0 /* unknown */
#define PTRACE_SETREGSET   0 /* unknown */
#define PTRACE_GETSIGINFO  0 /* unknown */
#define PTRACE_SETSIGINFO  0 /* unknown */
#define PTRACE_PEEKSIGINFO 0 /* unknown */
#define PTRACE_GETSIGMASK  0 /* unknown */
#define PTRACE_SETSIGMASK  0 /* unknown */
#define PTRACE_SETOPTIONS  0 /* unknown */
#define PTRACE_O_EXITKILL        0 /* unknown */
#define PTRACE_O_TRACECLONE      0 /* unknown */
#define PTRACE_O_TRACEEXEC       0 /* unknown */
#define PTRACE_O_TRACEEXIT       0 /* unknown */
#define PTRACE_O_TRACEFORK       0 /* unknown */
#define PTRACE_O_TRACESYSGOOD    0 /* unknown */
#define PTRACE_O_TRACEVFORK      0 /* unknown */
#define PTRACE_O_TRACEVFORKDONE  0 /* unknown */
#define PTRACE_O_TRACESECCOMP    0 /* unknown */
#define PTRACE_O_SUSPEND_SECCOMP 0 /* unknown */
#define PTRACE_GETEVENTMSG    0 /* unknown */
#define PTRACE_CONT           0 /* unknown */
#define PTRACE_SYSCALL        0 /* unknown */
#define PTRACE_SINGLESTEP     0 /* unknown */
#define PTRACE_SET_SYSCALL    0 /* unknown */
#define PTRACE_SYSEMU         0 /* unknown */
#define PTRACE_SYSEMU_SINGLESTEP 0 /* unknown */
#define PTRACE_LISTEN         0 /* unknown */
#define PTRACE_KILL           0 /* unknown */
#define PTRACE_INTERRUPT      0 /* unknown */
#define PTRACE_ATTACH         0 /* unknown */
#define PTRACE_SEIZE          0 /* unknown */
#define PTRACE_SECCOMP_GET_FILTER   0 /* unknown */
#define PTRACE_DETACH            0 /* unknown */
#define PTRACE_GET_THREAD_AREA   0 /* unknown */
#define PTRACE_SET_THREAD_AREA   0 /* unknown */
#define PTRACE_GET_SYSCALL_INFO  0 /* unknown */
#define PTRACE_SYSCALL_INFO_ENTRY   0 /* unknown */
#define PTRACE_SYSCALL_INFO_EXIT 0 /* unknown */
#define PTRACE_SYSCALL_INFO_SECCOMP 0 /* unknown */
#define PTRACE_SYSCALL_INFO_NONE 0 /* unknown */

typedef long off_t;
typedef int pid_t;

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int   mprotect(void *addr, size_t size, int prot);
int   munmap(void *addr, size_t length);

int   open(const char *pathname, int flags, ... /* mode_t mode */ );
int   write(int fd, const void *buf, size_t len);
off_t lseek(int fildes, off_t offset, int whence);
int   close(int fd);

long int ptrace(int op, pid_t pid, ... /* void *addr, void *data */ );

#endif /* KERNEL_H */
