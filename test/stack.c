#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#include "object.h"
#include "stack.h"

struct node { void *link; };

typedef struct stack {
   void *class;
   struct node *top, *last;
} stack_t;

void *stack_ctor(void *_self, [[maybe_unused]] va_list *args){
   stack_t *stack = _self;
   stack->top = NULL;
   stack->last = NULL;
   return stack;
}
void *stack_dtor(void *_self){
   return _self;
}

const void *Stack = &((struct Class){
   .alignment = alignof(stack_t),
   .size = sizeof(stack_t),
   .ctor = stack_ctor,
   .dtor = stack_dtor,
   .id = UINT32_C(0)
});
