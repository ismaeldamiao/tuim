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
   Driver to compile C using the Tuim Developer Kit.
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
   const char as[] = "clang -cc1as";
   char **input;
   char *args_out, *target;
   const char args_default[] = " "
      "-filetype obj "
      "-mrelocation-model pic ";
   char *tuim_home;

   return_value = EXIT_SUCCESS;
   print_invocation = false;
   args_out = NULL;
   target = tuim_target();
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
         }else if(STRCMP(argv[i], "--target")){
            ++i;
            target = argv[i];
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

   /* --- Get the command len --- */
   len = sizeof(as) + sizeof(args_default);
   len += sizeof("-triple") + strlen(target);
   ++len;
   for(char **buf = input; *buf != NULL; ++buf) ++len, len += strlen(*buf);
   if(args_out != NULL){
      len += sizeof("-o");
      ++len, len += strlen(args_out);
   }

   cmd = malloc(len);
   if(input == NULL){
      fprintf(stderr, "%s: malloc returned nullptr.\n", argv[0]);
      return_value = EXIT_FAILURE;
      goto end;
   }

   /* --- Write the command --- */
   strcpy(cmd, as);
   strcat(cmd, args_default);
   strcat(cmd, "-triple ");
   sprintf(cmd, "%s %s ", cmd, target);
   if(args_out != NULL) sprintf(cmd, "%s-o %s ", cmd, args_out);
   for(char **buf = input; *buf != NULL; ++buf)
      sprintf(cmd, "%s%s ", cmd, *buf);

   /* --- Execute --- */
   if(print_invocation) puts(cmd);
   system(cmd);

   end: {
      ptrarray_free(input, free);
      free(cmd);
   }

   return return_value;
}
