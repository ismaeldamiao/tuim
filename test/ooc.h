#ifndef OBJECT_H
#define OBJECT_H

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

typedef void *ctor_t(void *instance, va_list *args);
typedef void *dtor_t(void *instance);

#define ClassOf(instance) (*(void**)(instance))

#define class_id(p)        (((uintptr_t*)(p))[0])
#define class_alignment(p) (((uintptr_t*)(p))[1])
#define class_size(p)      (((uintptr_t*)(p))[2])
#define class_vtable(p)    ((void*)((uintptr_t*)(p) + 3))

/* class_id(some_class) == 0 ? */
#define class_ctor(p)      ((ctor_t*)(void*)((uintptr_t*)(p) + 3))
#define class_dtor(p)      ((dtor_t*)(void*)((uintptr_t*)(p) + 4))

#define new(class, ...) __tuim_new(class __VA_OPT__(,) __VA_ARGS__)
[[maybe_unused]] static
void *__tuim_new(void *class, ...){
   void *aligned_alloc(size_t,size_t);
   ctor_t *ctor = class_ctor(class);

   void *instance;
   va_list args;

   instance = aligned_alloc(class_alignment(class), class_size(class));

   va_start(args, class);
   instance = ctor(instance, &args);
   va_end(args);

   return instance;
}


#define delete(instance) __tuim_delete(instance)
[[maybe_unused]]
void __tuim_delete(void *instance){
   void free(void*);
   dtor_t *dtor = class_dtor(ClassOf(instance));
   if(instance != NULL){
      const struct Class *class = *(const void **)instance;
      free(dtor(instance));
   }
}

#endif // OBJECT_H
