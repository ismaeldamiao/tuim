/* *****************************************************************************
   MIT License

   Copyright (c) 2024 I.F.F. dos Santos <ismaellxd@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the “Software”), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
***************************************************************************** */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef STATIC
#define STATIC
#endif

/* ------------------------------------
   Function to get all path where a shared object can be found.
   * Part of tuim project.
   * Last modified: Octubre 11, 2024.
------------------------------------ */

/* In system that do not use char as ASCII these macro need
   to be changed to a function. */
#define ascii(c) ((uint8_t*)(c))
#define str(i) ((char*)(i))

STATIC char** path_search(uint8_t *soname){
   uint8_t *env, *env_cpy, *tmp0, *tmp1, *tmp2;
   char **path;
   bool new_str;
   size_t len, count, soname_len, i, j;
#if defined(TUIM_HOME)
   const size_t predef_count = (size_t)1;
#else
   const size_t predef_count = (size_t)0;
#endif // defined(TUIM_HOME)

   count = (size_t)0;
   env = NULL;

   tmp0 = tmp1 = tmp2 = ascii(getenv("LD_LIBRARY_PATH"));

   if(tmp0){
      len = (size_t)0;
      for(; *tmp0; ++tmp0) ++len;

      env = env_cpy = malloc(len);
      if(env == NULL) return NULL;

      new_str = true;
      for(;*tmp1; ++tmp1, ++env_cpy){
         if((*tmp1 == UINT8_C(0x3a)) || (*tmp1 == UINT8_C(0x3b))){
            new_str = true;
            *env_cpy = UINT8_C(0x00);
         }else{
            if(new_str){
               new_str = false;
               ++count;
            }
            *env_cpy = *tmp1;
         }
      }
   }

   soname_len = strlen(str(soname));
   path = malloc((count + predef_count + 1) * sizeof(char*));
   if(path == NULL){
      free(env);
      return NULL;
   }

   for(i = (size_t)0, j = (size_t)0; i < count; ++j){
      if(*(env + j)){
         len = strlen(str(env + j));
         path[i] = malloc(len + soname_len + 2);
         strcpy(path[i], str(env + j));
         strcat(path[i], "/");
         strcat(path[i], str(soname));
         j += len;
         ++i;
      }
   }
#if defined(TUIM_HOME)
   path[i] = malloc(sizeof(TUIM_HOME "/lib/") + soname_len + 1);
   strcpy(path[i], TUIM_HOME "/lib/");
   strcat(path[i], str(soname));
   ++i;
#endif // defined(TUIM_HOME)
   path[i] = NULL;

   free(env);
   return path;
}
