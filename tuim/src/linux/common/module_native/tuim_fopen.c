/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 06, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

/*******************************************************************************
 * Function to find end open object files for read. The function also create
 * a memory map for the file.
 ******************************************************************************/
static int  tuim_fopen(tuim_elf *file, const char *file_name){
   int fd;
   size_t length;
   int prot, flags;
   const off_t offset = (off_t)0;
   off_t position_indicator_bak;
   void *ptr;

   fd = open(file_name, O_RDONLY);
   if(fd == -1){
      return TUIM_ENOENT;
   }

   file->fd = fd;

   /* compute the file length */

   position_indicator_bak = lseek(fd, offset, SEEK_CUR);
   length = lseek(fd, offset, SEEK_END);
   lseek(fd, position_indicator_bak, SEEK_SET);

   file->sz = length;

   /* map the file */

   prot = PROT_READ | PROT_WRITE;
   flags = MAP_PRIVATE;
   ptr = mmap(NULL, length, prot, flags, fd, offset);
   if(ptr == MAP_FAILED){
      return TUIM_ENOMEM;
   }

   file->map = ptr;

   /* */

   ptr = malloc(strlen(file_name) + 1);
   if(ptr == NULL){
      return TUIM_ENOMEM;
   }
   strcpy(ptr, file_name);

   file->path = ptr;

   return TUIM_SUCCESS;
}

static void tuim_fclose(tuim_elf *file){
   free(file->path);
   munmap(file->map, file->sz);
   close(file->fd);
}
