#include <stddef.h>
#include "stdlib_impl.h"

typedef struct free_block {
    size_t size;
    struct free_block* next;
} free_block;

static free_block free_block_list_head = { 0, 0 };
static const size_t overhead = sizeof(size_t);
static const size_t align_to = 16;

void* malloc(size_t size) {
   size = (size + sizeof(size_t) + (align_to - 1)) & ~ (align_to - 1);
   free_block* block = free_block_list_head.next;
   free_block** head = &(free_block_list_head.next);
   while (block != 0) {
      if (block->size >= size) {
         *head = block->next;
         return ((char*)block) + sizeof(size_t);
      }
      head = &(block->next);
      block = block->next;
   }

   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      int prot, flags;
      off_t offset = (off_t)0;

      block = (free_block*)mmap(NULL, size, prot, flags, 0, offset);
   #endif
   block->size = size;

   return ((char*)block) + sizeof(size_t);
}
