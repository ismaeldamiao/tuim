#include <stddef.h>
#include <stdint.h>

#define SIZE_C(x) ((size_t)x##U)

static void *_memcpy(void * restrict s1, const void * restrict s2, size_t n){
   uint8_t *_s1 = s1;
   const uint8_t *_s2 = s2;

   if(n == SIZE_C(0)) return s1;

   loop: {
      *_s1 = *_s2;
      --n;
      if(n > SIZE_C(0)){
         ++_s1, ++_s2;
         goto loop;
      }
   }
   return s1;
}

static void *_memset(void *s, int c, size_t n){
   uint8_t *_s = s;
   const uint8_t _c = (uint8_t)c;

   if(n == SIZE_C(0)) return s;

   loop: {
      *_s = _c;
      --n;
      if(n > SIZE_C(0)){
         ++_s;
         goto loop;
      }
   }
   return s;
}
