/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 07, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

/*******************************************************************************
 * Function to write on execution context address space.
 *
 * Current implementation uses the file /pro/<PID>/mem.
 * FIXME: write() call is not handling partial writes.
 ******************************************************************************/
static int tuim_write(tuim_ctx *ctx, uintptr_t address, size_t size, const void *buf){
   pid_t pid;
   int fd;
   char file_name[0x40];
   union aliasing { off_t offset; uintptr_t address; } aliasing;

   pid = ctx->pid;
   sprintf(file_name, "/proc/%d/mem", (int)pid);
   fd = open(file_name, O_RDWR);
   if(fd == -1){
      ctx->strerror = strerror(errno);
      return 1;
   }

   aliasing.address = address;
   if(lseek(fd, aliasing.offset, SEEK_SET) == (off_t)(-1)){
      close(fd);
      ctx->strerror = strerror(errno);
      return 1;
   }

   if(write(fd, buf, size) == (ssize_t)(-1)){
      close(fd);
      ctx->strerror = strerror(errno);
      return TUIM_EFAULT;
   }
   close(fd);

   return TUIM_SUCCESS;
}
