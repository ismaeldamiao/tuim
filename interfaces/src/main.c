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
#include "tuim.h"
/* ------------------------------------
   This is the command line front-end for the Tuim project.
   * Part of Tuim Project.
   * Last modified: February 21, 2025.
------------------------------------ */

//#include "main.h"

int main(int argc, char **argv){
   int tuim_errno;
   char *tuim_home;

   if(argc < 2){
      return TUIM_EINVAL;
   }

   tuim_home = getenv("TUIM_HOME");
   if(tuim_home == NULL){
      fputs("WARNING: Environmet variable TUIM_HOME unset\n", stderr);
   }

   tuim_setenv(TUIM_HOME, tuim_home);
   tuim_setenv(LD_LIBRARY_PATH, getenv("LD_LIBRARY_PATH"));

   /* Validate arguments. */
   if(strcmp(argv[1], "run") == 0){
      if((argv[2][0] == '-') && (argv[2][1] == 'm'))
         tuim_errno = tuim_run(argv[3], argv + 4);
      else
         tuim_errno = tuim_run(NULL, argv + 2);
   }else if(strcmp(argv[1], "build") == 0){
      tuim_errno = tuim_build(argv + 2);
   }else if(strcmp(argv[1], "install") == 0){
      tuim_errno = tuim_install(argv + 2);
   }else if(strcmp(argv[1], "remove") == 0){
      tuim_errno = tuim_remove(argv + 2);
   }else{
      fputs("Unknown argument ", stderr);
      fputs(argv[1], stderr);
      fputs("\n", stderr);
      tuim_errno = TUIM_EINVAL;
   }

   return tuim_errno;
}

#include "tuim_setenv.c"
#include "tuim_run.c"
#include "tuim_build.c"
#include "tuim_install.c"
#include "tuim_remove.c"
