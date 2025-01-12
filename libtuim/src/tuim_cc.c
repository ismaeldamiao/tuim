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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/tuim.h"
#include "tuim_private.h"
#include "elf.h"
/* ------------------------------------
   Function to compile C source code.
   TODO: Use libclang and LLVM-C libraries instead of system.
   * Part of tuim project.
   * Last modified: January 3, 2025.
------------------------------------ */
tuim_elf* tuim_cc(void *input, const char *target, const char *include, uint64_t args){
   char *cmd;
   size_t cmd_len;
   int argc;
   tuim_elf *elf;
   const char *argv[] = {
      "-std=c23",
      "-pedantic",
      "-ffreestanding",
      "-emit-obj",
      "-pic-level",
      "2",
      "-triple",
      target,
      //"-fno-builtin"
      "-nostdsysteminc",
      "-isystem",
      strcat_(getenv("TUIM_HOME"), "/include"), // Must be the last
   };
   argc = sizeof(argv) / sizeof(char*);
   if(argv[argc-1] == NULL) argc -= 2;

   cmd_len = sizeof("clang -cc1");
   for(int i = 0; i < argc; ++i){
      cmd_len += strlen(argv[i]);
      ++cmd_len;
   }
   if(args & TUIM_CC_FILE){
      cmd_len += strlen(input);
      ++cmd_len;
   }
   cmd_len += sizeof("-o output.o");
   ++cmd_len;

   cmd = malloc(cmd_len);
   if(cmd == NULL){
      tuim_error = TUIM_ERROR_MEMORY;
      return NULL;
   }

   strcat(cmd, "clang -cc1");
   for(int i = 0; i < argc; ++i){
      sprintf(cmd, "%s %s", cmd, argv[i]);
   }
   sprintf(cmd, "%s -o output.o %s", cmd, (char*)input);

   //puts(cmd);
   system(cmd);
   free(cmd);

   {
      FILE *file;
      int status;
      file = fopen("output.o", "rb");
      if(file == NULL){
         tuim_error = TUIM_ERROR_READING;
         return NULL;
      }
      
      fseek(file, 0L, SEEK_END);
      long file_size = ftell(file);
      rewind(file);
      
      elf = malloc(sizeof(tuim_elf));
      if(elf == NULL){
         fclose(file);
         tuim_error = TUIM_ERROR_MEMORY;
         return NULL;
      }
      elf->program = malloc((size_t)file_size);
      if(elf->program == NULL){
         fclose(file);
         tuim_error = TUIM_ERROR_MEMORY;
         return NULL;
      }

      status = fread(
         elf->program,
         sizeof(unsigned char),
         file_size,
         file
      );
      elf->start_vaddr = (typeof(elf->start_vaddr))file_size;

      fclose(file);
      remove("output.o");

      if(status < 0){
         tuim_error = TUIM_ERROR_READING;
         free(elf->program);
         free(elf);
         return NULL;
      }
   }

   return elf;
}
