#ifndef PTRARRAY_H
#define PTRARRAY_H

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------
   Header-only library to work with NULL-terminared arrays of pointers.
   * Last modified: January 3, 2025.
------------------------------------ */

#if __STDC_VERSION__ >= 202311L
   #define maybe_unused [[maybe_unused]]
#else
   #define maybe_unused
#endif

maybe_unused
static void* ptrarray_new(void){
   void **ptr;
   ptr = malloc(sizeof(void*));
   if(ptr != NULL){
      *ptr = NULL;
   }
   return ptr;
}

maybe_unused
static void* ptrarray_grow(void *dest, const void *src, size_t size){
   void **ptr;
   size_t i, newsize;

   i = (size_t)0;
   ptr = dest;
   while(ptr[i] != NULL) ++i;
   newsize = sizeof(void*) * (i + (size_t)2);
   ptr = realloc(dest, newsize);
   if(ptr != NULL){
      ptr[i] = malloc(size);
      if(ptr[i] != NULL){
         if(src != NULL) memcpy(ptr[i], src, size);
      }
      ptr[++i] = NULL;
      return ptr;
   }
   return ptr;
}

maybe_unused
static void* ptrarray_strgrow(char **dest, const char *src){
   return ptrarray_grow(dest, src, strlen(src) + (size_t)1);
}

maybe_unused
static void ptrarray_free(void *ptr, void(*free_)(void*)){
   if(ptr == NULL) return;
   if((void*)free_ != NULL){
      for(void **ptr_ = ptr; *ptr_ != NULL; ++ptr_) free_(*ptr_);
   }
   free(ptr);
}

#undef maybe_unused

#endif /* PTRARRAY_H */
