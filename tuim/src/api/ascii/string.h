#ifndef ASCII_STRING_H
#define ASCII_STRING_H

#include <stdint.h>

#define SIZE_C(x) ((size_t)x##U)

#if __STDC_VERSION__ >= 202311L
   #define maybe_unused [[maybe_unused]]
#elif defined(__GNUC__)
   #define maybe_unused __attribute__((unused))
#endif

maybe_unused
static uint8_t *ascii_strcpy(uint8_t * restrict s1, const uint8_t * restrict s2){
   uint8_t *_s1 = s1;
   loop: {
      *_s1 = *s2;
      if(*s2 != UINT8_C(0x00)){
         ++_s1;
         ++s2;
         goto loop;
      }
   }
   return s1;
}

maybe_unused
static uint8_t *ascii_strcat(uint8_t * restrict s1, const uint8_t * restrict s2){
   uint8_t *_s1 = s1;
   while(*_s1 != UINT8_C(0x00)){
      ++_s1;
   }
   ascii_strcpy(_s1, s2);
   return s1;
}

maybe_unused
static size_t ascii_strlen(const uint8_t *s){
   const uint8_t * const s_cpy = s;
   while(*s != UINT8_C(0x00)) ++s;
   return (size_t)(s - s_cpy);
}

maybe_unused
static int ascii_strcmp(const uint8_t *s1, const uint8_t *s2){
   for(; (*s1 == *s2) && (*s1 != UINT8_C(0x00)); ++s1, ++s2);
   return (int)(*s1 - *s2);
}

#endif /* ASCII_STRING_H */
