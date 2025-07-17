#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#if 1
int fileno(FILE *stream);
off_t lseek(int fildes, off_t offset, int whence);
void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
#endif

void *fmap(FILE *stream){
   int fd;
   size_t length;
   int prot, flags;
   const off_t offset = (off_t)0;
   off_t position_indicator_bak;
   void *ptr;

   /* get the file descriptor associeated with the stream */

   fd = fileno(stream);

   /* compute the file length */

   position_indicator_bak = lseek(fd, offset, SEEK_CUR);
   length = lseek(fd, offset, SEEK_END);
   lseek(fd, position_indicator_bak, SEEK_SET);

   /* map the file */

   prot = PROT_READ | PROT_WRITE;
   flags = MAP_PRIVATE;
   ptr = mmap(NULL, length, prot, flags, fd, offset);

   return ptr;
}
