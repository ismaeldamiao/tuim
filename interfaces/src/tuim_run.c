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
#include "tuim.h"
#include "main.h"
/* ------------------------------------
   This function call the ELF interpreter and execute the program.
   * Part of Tuim Project.
   * Last modified: February 21, 2025.
------------------------------------ */

int tuim_run(const char *machine, char **argv){
   int tuim_errno;
   if(machine == NULL){
      elf_s *executable = NULL;
      tuim_errno = tuim_interpreter(ascii(argv[0]), &executable);
      if(tuim_errno != TUIM_SUCCESS){
         tuim_free(executable);
         return tuim_errno;
      }
      //tuim_errno = tuim_exec(executable);
      (executable->entry)();
      tuim_free(executable);
      return tuim_errno;
   }else{
      // TODO: Handle the machine loading and running
   }
   return TUIM_SUCCESS;
}

#include "tuim_interpreter.c"
#include "tuim_free.c"
