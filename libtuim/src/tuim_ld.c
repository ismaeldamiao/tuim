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
/* ------------------------------------
   Function to compile C source code.
   TODO: Use libclang and LLVM-C libraries instead of system.
   * Part of tuim project.
   * Last modified: January 3, 2025.
------------------------------------ */

static bool are_args_valid(const char *arg){
   if(arg == NULL) return true;
   return true;
}

static bool is_shared(const char *arg){
   const char shared[] = " --shared ";
   const size_t arg_len = strlen(arg);
   const uintptr_t arg_end = (uintptr_t)arg + (uintptr_t)arg_len;
   bool maybe;

   for(const char *str = arg; *str != '\0'; ++str){
      uintptr_t str_end = (uintptr_t)str + sizeof(shared);
      if(str_end <= arg_end){
         maybe = true;
         for(size_t i = (size_t)0; i < sizeof(shared); ++i){
            if(str[i] != shared[i]){
               maybe = false;
               break;
            }
         }
         if(!maybe) continue;
      }
   }
   return false;
}

tuim_elf* tuim_ld(void *input, unsigned flags, char *arg){
   char *cmd;
   const char default_args[] =
      "--oformat=elf "
      "--image-base=0x0 "
      "-z separate-code "
      "-z separate-loadable-segments "
      "--nostdlib "
      // Disable non standard ELF features
      "--hash-style=sysv " // do not use SHT_GNU_HASH
      "-z nognustack " // do not use SHT_GNU_HASH
      "-z norelro " // do not use PT_GNU_RELRO
      "--no-eh-frame-hdr "; // do not use PT_GNU_EH_FRAME
   char *stdlib, *tuim_home;

   if(!are_args_valid(arg)){
      tuim_error = TUIM_ERROR_INVALIDELF;
      return NULL;
   }

   tuim_home = getenv("TUIM_HOME");
   if(tuim_home == NULL){
      tuim_error = TUIM_ERROR_READING;
      return NULL;
   }

   if(flags & TUIM_LD_ELF){
      // ...
   }else{
      cmd = malloc(sizeof(default_args) + strlen(arg));
      if(cmd == NULL){
         tuim_error = TUIM_ERROR_MEMORY;
         return NULL;
      }
      strcpy(cmd, default_args);
      strcat(cmd, arg);

      puts(cmd);
      system(cmd);
      free(cmd);
   }

#if 0
   size_t cmd_len;
   int argc;
   tuim_elf *elf;
   const char *argv[] = {
      "--oformat=elf",
      "--image-base=0x0",
      "-z", "separate-code",
      "-z", "separate-loadable-segments",
      // Disable non standard ELF features
      "--hash-style=sysv", // do not use SHT_GNU_HASH
      "-z", "nognustack", // do not use SHT_GNU_HASH
      "-z", "norelro", // do not use PT_GNU_RELRO
      "--no-eh-frame-hdr", // do not use PT_GNU_EH_FRAME
      "-e", "main",
      "-dynamic-linker", "/usr/bin/tuim",
      "--nostdlib",
      "-L",
      strcat_(getenv("TUIM_HOME"), "/lib"), // Must be the last
   };
   argc = sizeof(argv) / sizeof(char*);
   if(args & TUIM_LD_SHARED){
      argv[argc-4] = argv[argc-5] = "\0";
      argv[argc-6] = "--shared";
      argv[argc-7] = "-Bsymbolic";
   }else{
      if(entry != NULL) argv[argc-6] = entry;
   }
   if(argv[argc-1] == NULL) argc -= 2;

   cmd_len = sizeof("ld.lld");
   for(int i = 0; i < argc; ++i){
      cmd_len += strlen(argv[i]);
      ++cmd_len;
   }
   if(libs != NULL){
      for(char **str = libs; *str != NULL; ++str){
         cmd_len += strlen(*str);
         cmd_len += (size_t)3;
      }
   }
   if(args & TUIM_LD_FILE){
      for(const char **str = input; *str != NULL; ++str){
         cmd_len += strlen(*str);
         ++cmd_len;
      }
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

   strcat(cmd, "ld.lld");
   for(int i = 0; i < argc; ++i){
      sprintf(cmd, "%s %s", cmd, argv[i]);
   }
   sprintf(cmd, "%s -o libout.so", cmd);
   if(libs != NULL){
      for(char **str = libs; *str != NULL; ++str){
         sprintf(cmd, "%s -l%s", cmd, *str);
      }
   }
   if(args & TUIM_LD_FILE){
      for(const char **str = input; *str != NULL; ++str){
         sprintf(cmd, "%s %s", cmd, *str);
      }
   }

   puts(cmd);
   system(cmd);
   free(cmd);
#endif
   {
      FILE *file;
      int status;
      file = fopen("libout.so", "rb");
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
      remove("libout.so");

      if(status < 0){
         tuim_error = TUIM_ERROR_READING;
         free(elf->program);
         free(elf);
         return NULL;
      }
   }

   return elf;

}
