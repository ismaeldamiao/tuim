/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 07, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

/*******************************************************************************
 * Function to write on execution context address space.
 *
 * Current implementation uses the file /pro/<PID>/mem.
 * FIXME: read() call is not handling partial reads.
 ******************************************************************************/
static int tuim_read(tuim_ctx *ctx, uintptr_t address, size_t size, void *buf){
   pid_t pid;
   int fd;
   char file_name[0x12];
   union aliasing { off_t offset; uintptr_t address; } aliasing;

   pid = ctx->pid;
   sprintf(file_name, "/proc/%d/mem", (int)pid);
   fd = open(file_name, O_RDONLY);
   if(fd == -1){
      return 1;
   }

   aliasing.address = address;
   if(lseek(fd, aliasing.offset, SEEK_SET) == (off_t)(-1)){
      close(fd);
      return 1;
   }

   if(read(fd, buf, size) == (ssize_t)(-1)){
      close(fd);
      return TUIM_EFAULT;
   }
   close(fd);

   return TUIM_SUCCESS;
}
