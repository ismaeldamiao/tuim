#ifndef STACK_H
#define STACK_H

extern const void *Stack;

void stack_push(void *stack, void *element);
void *stack_pop(void *stack);

#endif // STACK_H
