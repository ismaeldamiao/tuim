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
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#if __STDC_VERSION__ >= 202311L && __has_include(<tuim.h>)
#include <tuim.h>
#else
#include "../api/tuim.h"
#endif
/* ------------------------------------
   This is the command line front-end for the Tuim project.
   * Part of Tuim Project.
   * Last modified: July 07, 2025.
------------------------------------ */

int main(int argc, char **argv){
   tuim_ctx *ctx;
   char *machine = NULL, *exec_path = NULL;
   char *env, tuim_home[512], ld_library_path[512];
   char memory_buffer[512];
   int arg0 = 1;
   int _errno;

   /* With Tuim's API one can start a application with three general steps:
      1. Create a new context using the machine and environment variables
      2. Load and dynamically link the executable
      3. Given control to the application */

   if(argc < 2){
      return TUIM_EINVAL;
   }

   /* First step: Create a new context
      Here I first get the value of environment variables, check if
      a machine shall be used and then create the context. */
   env = getenv("TUIM_HOME");
   if(env != NULL){
      strcpy(tuim_home, env);
   }else{
      fputs("WARNING: Environmet variable TUIM_HOME not set\n", stderr);
   }
   env = getenv("LD_LIBRARY_PATH");
   if(env != NULL){
      strcpy(ld_library_path, env);
   }

   if((argc > 2) && (argv[1][0] == '-') && (argv[1][1] == 'm')){
      machine = argv[2];
      arg0 = 3;
   }
   ctx = tuim_newcontext(tuim_home, ld_library_path, machine);
   if(ctx == NULL){
      return TUIM_ENOMEM;
   }else if((_errno = tuim_errno(ctx)) != TUIM_SUCCESS){
      fputs(tuim_strerror(ctx), stderr);
      fputs("\n", stderr);
      tuim_deletecontext(ctx);
      return _errno;
   }

   /* Second step: Load and link
      Here I check the executable's path based on command line arguments,
      load it and link with shared libraries. */
   exec_path = tuim_getpathexec(ctx, argv[arg0]);
   strcpy(memory_buffer, exec_path);
   exec_path = memory_buffer;

   tuim_loader(ctx, exec_path);
   if((_errno = tuim_errno(ctx)) != TUIM_SUCCESS){
      fputs(tuim_strerror(ctx), stderr);
      fputs("\n", stderr);
      tuim_deletecontext(ctx);
      return _errno;
   }

   tuim_linker(ctx, exec_path);
   if((_errno = tuim_errno(ctx)) != TUIM_SUCCESS){
      fputs(tuim_strerror(ctx), stderr);
      fputs("\n", stderr);
      tuim_deletecontext(ctx);
      return _errno;
   }

   /* Thrird step: Execute */
   _errno = tuim_exec(ctx, exec_path, argv + arg0, NULL);
   if((_errno = tuim_errno(ctx)) != TUIM_SUCCESS){
      fputs(tuim_strerror(ctx), stderr);
      fputs("\n", stderr);
      tuim_deletecontext(ctx);
      return _errno;
   }

#if 0
   tuim_deletecontext(ctx);
#endif
   return _errno;
}
