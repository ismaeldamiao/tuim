#ifndef OBJECT_H
#define OBJECT_H

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

struct Class {
   size_t alignment, size;
   void *(*ctor)(void *, va_list *);
   void *(*dtor)(void *);
   uint32_t id;
};

void *new(const void *class, ...);
void delete(void *instance);

size_t AlignOf(const void *instance);
size_t SizeOf (const void *instance);

void *ctor(void *instance, ...);
void *dtor(void *instance);

#endif // OBJECT_H
