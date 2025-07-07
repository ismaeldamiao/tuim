#if defined(__ARM_ARCH) && (__ARM_ARCH <= 7)
// AARCH32
// arch/arm/tools/syscall.tbl
   #define __NR_write 4
#elif defined(__i386__)
// i386
// arch/x86/entry/syscalls/syscall_32.tbl
   #define __NR_write 4
#elif defined(__x86_64__)
// AMD64
// arch/x86/entry/syscalls/syscall_64.tbl
   #define __NR_write 1
#else
// scripts/syscall.tbl
// include/uapi/asm-generic/unistd.h
// RISC-V
// AARCH64

   #define __NR_read            63
   #define __NR_write           64
   #define __NR_open            56
   #define __NR_close           57
   #define __NR_stat            79   /* __NR3264_fstat */
   #define __NR_fstat           80   /* __NR3264_fstat */
   #define __NR_lseek           62   /* __NR3264_lseek */
   #define __NR_mkdir           83
   #define __NR_rmdir           /* not in generic? use openat + unlinkat */
   #define __NR_fork            1079

   /* Read/write vectors */
   #define __NR_readv           65
   #define __NR_writev          66
   #define __NR_pread64         67
   #define __NR_pwrite64        68

   /* File descriptor operations */
   #define __NR_pipe2           59
   #define __NR_dup2            1041
   #define __NR_fsync           82
   #define __NR_fdatasync       83
   #define __NR_sync            81

   /* File tree / directory */
   #define __NR_getdents64      61
   #define __NR_openat          56
   #define __NR_close           57

   /* Extended file operations */
   #define __NR_ftruncate       84   /* generic sync_file_range or ftruncate */
   #define __NR_truncate        84

   /* Time / waiting */
   #define __NR_select          1067
   #define __NR_poll            1068
   #define __NR_epoll_wait      1069
   #define __NR_vfork           1071
   #define __NR_wait4           1072  /* oldwait4 */

   /* Memory management */
   #define __NR_brk             214
   #define __NR_mmap            __NR3264_mmap  /* 222 */
   #define __NR_munmap          215
   #define __NR_mremap          216
   #define __NR_mprotect        226
   #define __NR_msync           227
   #define __NR_mlock           228
   #define __NR_munlock         229
   #define __NR_mlockall        230
   #define __NR_munlockall      231
   #define __NR_mincore         232
   #define __NR_madvise         233
   #define __NR_remap_file_pages 234
   #define __NR_swapon          224
   #define __NR_swapoff         225

   /* Process control */
   #define __NR_clone           220
   #define __NR_execve          221
#endif /* defined(__riscv) */
