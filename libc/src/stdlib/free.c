#include <stddef.h>

typedef struct free_block {
    size_t size;
    struct free_block* next;
} free_block;

static free_block free_block_list_head = { 0, 0 };
static const size_t overhead = sizeof(size_t);
static const size_t align_to = 16;

void free(void *ptr){
   free_block* block = (free_block*)(((char*)ptr) - sizeof(size_t));
   block->next = free_block_list_head.next;
   free_block_list_head.next = block;
}
