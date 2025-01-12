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
#include <tuim.h>

/* ------------------------------------
   Interface for Tuim's ELF loader, to execute program from command line.
   * Part of tuim project.
   * Last modified: January 11, 2025.
------------------------------------ */

int main(int argc, char **argv){
   int i, prog, *return_code;
   tuim_elf *elf;
   struct args { int argc; char **argv; } args;

   /* Validate arguments. */
   prog = 0;
   for(i = 1; i < argc; ++i){
      if(argv[i][0] == '-'){}else{
         prog = i;
         break;
      }
   }
   if(prog == 0) return 0;

   /* Load the executable, referenced in argv[prog]. */
   elf = tuim_loader(argv[prog], TUIM_LOADER_FEXEC);
   if(elf == NULL){
      tuim_perror(argv[0]);
      return 127;
   }

   /* Execute */
   args.argc = argc - prog;
   args.argv = argv + prog;
   return_code = tuim_exec(TUIM_EXEC_TC, TUIM_EXEC_FNONE, elf, &args);
   if(return_code == NULL){
      tuim_perror(argv[0]);
      return 127;
   }

   /* Free the memory and return. */
   tuim_free(elf);
   return *return_code;
}
