#include <stdio.h>
#include "object.h"
#include "stack.h"

static char in[] = "Ola Mundo!";

int main(void){
   void *stack = new(Stack);
   char *out;

   stack_push(stack, in);
   out = stack_pop(stack);

   puts(out);

   delete(stack);
   return 0;
}
