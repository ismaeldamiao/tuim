/* *****************************************************************************
   MIT License

   Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>

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
#include <stdint.h>
#include <string.h>
#include "tuim.h"
#include "main.h"
/* ------------------------------------
   This function set up environment variables used by the interpreter.
   * Part of Tuim Project.
   * Last modified: February 21, 2025.
------------------------------------ */

uint8_t *tuim_env[2] = { };
size_t tuim_ld_library_path_len = 0;

int tuim_setenv(int env, char *value){

   if(env == LD_LIBRARY_PATH){
      tuim_ld_library_path_len = strlen(value);
      for(char *str = value; *str != '\0'; ++str){
         if((*str == ':') || (*str == ';')) *str = '\0', ++str;
      }
   }

   tuim_env[env] = ascii(value);

   return TUIM_SUCCESS;
}
