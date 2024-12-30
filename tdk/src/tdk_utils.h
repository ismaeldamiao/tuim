#ifndef TDK_UTILS_H
#define TDK_UTILS_H 1

#include <stdlib.h>

static void* ReallocString(char *dest, const char *src){
   void *ptr;
   ptr = realloc(dest, strlen(src) + (size_t)1);
   if(ptr != NULL){
      strcpy(ptr, src);
      return ptr;
   }else
      return dest;
}

static void* GrowPtrArray(void *dest, const void *src, size_t size){
   void **ptr;
   size_t i, newsize;

   i = (size_t)0;
   ptr = dest;
   while(ptr[i] != NULL) ++i;
   newsize = sizeof(void*) * (i + (size_t)2);
   ptr = realloc(dest, newsize);
   if(ptr != NULL){
      ptr[i] = malloc(size);
      if((ptr[i] != NULL) && (src != NULL)) memcpy(ptr[i], src, size);
      ptr[++i] = NULL;
      return ptr;
   }else
      return dest;
}

static void* GrowStringArray(char **dest, const char *src){
#if 1
   return GrowPtrArray(dest, src, strlen(src) + (size_t)1);
#else
   char **ptr;
   size_t i, newsize;

   i = (size_t)0;
   while(dest[i] != NULL) ++i;
   newsize = sizeof(char*) * (i + (size_t)2);
   ptr = realloc(dest, newsize);
   if(ptr != NULL){
      ptr[i] = malloc(strlen(src) + (size_t)1);
      if(ptr[i] != NULL) strcpy(ptr[i], src);
      ptr[++i] = NULL;
      return ptr;
   }else
      return dest;
#endif
}

static void FreePtrArray(void *ptr){
   void **ptr_;
   ptr_ = ptr;
   while(*ptr_ != NULL){
      free(*ptr_);
      ++ptr_;
   }
   free(ptr);
}

#endif // TDK_UTILS_H