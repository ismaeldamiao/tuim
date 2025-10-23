#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include "object.h"

void *aligned_alloc(size_t,size_t);

size_t AlignOf(const void *instance){
   const struct Class *class = *(const void **)instance;
   return class->alignment;
}
size_t SizeOf (const void *instance){
   const struct Class *class = *(const void **)instance;
   return class->size;
}

void *ctor(void *instance, ...){
   const struct Class *class = *(const void **)instance;
   va_list args;
   va_start(args, instance);
   instance = class->ctor(instance, &args);
   va_end(args);
   return instance;
}
void *dtor(void *instance){
   const struct Class *class = *(const void **)instance;
   return class->dtor(instance);
}

void *new(const void *_class, ...){
   const struct Class *class = _class;
   void *instance;
   va_list args;

   instance = aligned_alloc(class->alignment, class->size);
   if(instance == NULL){
      // a particular language may instead "thow exception", "cath error", etc...
      return NULL;
   }
   *(const void **)instance = _class;

   va_start(args, _class);
   instance = class->ctor(instance, &args);
   va_end(args);

   return instance;
}

void delete(void *instance){
   if(instance != NULL){
      const struct Class *class = *(const void **)instance;
      free(class->dtor(instance));
   }
}
