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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tuim.h>
#include "print_version.h"
#include "ptrarray.h"
/* ------------------------------------
   Driver to link objects using the Tuim Developer Kit.
   * Part of tuim project.
   * Last modified: January 6, 2025.
------------------------------------ */

#define STRCMP(lhs, rhs) (strcmp((char*)(lhs), (char*)(rhs)) == 0)

#ifndef STATIC
#define STATIC
#endif

#ifndef MAIN
#define MAIN main
#endif

STATIC int MAIN(int argc, char **argv){
   char *cmd;
   size_t len;
   int return_value;
   int shared;
   bool print_invocation;
   const char lld[] = "ld.lld";
   char **input;
   char *args_out, **args_libs;
   char *args_entry = "main"; // Default entry point
   const char args_default[] = " "
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

   return_value = EXIT_SUCCESS;
   print_invocation = false;
   shared = 0; // Means that --shared option is not set
   args_out = NULL;
   args_libs = ptrarray_new();
   if(args_libs == NULL){
      fprintf(stderr, "%s: malloc returned nullptr.\n", argv[0]);
      return_value = EXIT_FAILURE;
      goto end;
   }
   input = ptrarray_new();
   if(input == NULL){
      fprintf(stderr, "%s: malloc returned nullptr.\n", argv[0]);
      return_value = EXIT_FAILURE;
      goto end;
   }

   /* Parse arguments and count how many sources must be compiled. */
   for(int i = 1; i < argc; ++i){
      if(argv[i][0] == '-'){
         if(STRCMP(argv[i], "--version")){
            print_version();
            return EXIT_SUCCESS;
         }else if(STRCMP(argv[i], "--shared")){
            shared = i;
         }else if(STRCMP(argv[i], "-e") || STRCMP(argv[i], "--entry")){
            ++i;
            args_entry = argv[i];
         }else if(argv[i][1] == 'l'){
            void *buf;
            buf = ptrarray_strgrow(args_libs, argv[i]);
            if(buf == NULL){
               fprintf(stderr, "%s: realloc returned nullptr.\n", argv[0]);
               return_value = EXIT_FAILURE;
               goto end;
            }
            args_libs = buf;
         }else if(STRCMP(argv[i], "-o")){
            ++i;
            args_out = argv[i];
         }else if(STRCMP(argv[i], "-v")){
            print_invocation = true;
         }else{
            fprintf(stderr, "%s: Unknown option %s.\n", argv[0], argv[i]);
            return EXIT_FAILURE;
         }
      }else{
         void *buf;
         buf = ptrarray_strgrow(input, argv[i]);
         if(buf == NULL){
            fprintf(stderr, "%s: realloc returned nullptr.\n", argv[0]);
            return_value = EXIT_FAILURE;
            goto end;
         }
         input = buf;
      }
   }

   tuim_home = getenv("TUIM_HOME");
   if(tuim_home == NULL){
      fprintf(
         stderr,
         "%s: Warning: Environment variable TUIM_HOME not set.\n",
         argv[0]
      );
   }

   /* --- Get the command len --- */
   len = sizeof(lld) + sizeof(args_default);
   for(char **buf = input; *buf != NULL; ++buf) ++len, len += strlen(*buf);
   for(char **buf = args_libs; *buf != NULL; ++buf) ++len, len +=strlen(*buf);
   if(shared != 0)
      len += sizeof("--shared");
   else{
      len += sizeof("-e");
      ++len, len += strlen(args_entry);
   }
   if(args_out != NULL){
      len += sizeof("-o");
      ++len, len += strlen(args_out);
   }
   if(tuim_home != NULL){
      len += sizeof("-L '/lib'");
      ++len, len += strlen(tuim_home);
   }

   cmd = malloc(len);
   if(input == NULL){
      fprintf(stderr, "%s: malloc returned nullptr.\n", argv[0]);
      return_value = EXIT_FAILURE;
      goto end;
   }

   /* --- Write the command --- */
   strcpy(cmd, lld);
   strcat(cmd, args_default);
   if(shared != 0) strcat(cmd, "--shared ");
   else sprintf(cmd, "%s-e %s ", cmd, args_entry);
   if(args_out != NULL) sprintf(cmd, "%s-o %s ", cmd, args_out);
   if(tuim_home != NULL)
      sprintf(cmd, "%s-L '%s/lib' ", cmd, tuim_home);
   for(char **buf = args_libs; *buf != NULL; ++buf)
      sprintf(cmd, "%s%s ", cmd, *buf);
   for(char **buf = input; *buf != NULL; ++buf)
      sprintf(cmd, "%s%s ", cmd, *buf);

   /* --- Execute --- */
   if(print_invocation) puts(cmd);
   system(cmd);

   end: {
      ptrarray_free(input, free);
      ptrarray_free(args_libs, free);
      free(cmd);
   }

   return return_value;
}
