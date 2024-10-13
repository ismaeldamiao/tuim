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
#include <stdio.h>
#include <stdlib.h>
#include <tuim.h>

/* ------------------------------------
   Interface for Tuim's ELF loader, to execute program from command line.
   * Part of tuim project.
   * Last modified: Octubre 11, 2024.
------------------------------------ */

int main(int argc, char **argv){
   tuim_elf *elf;
   void* main_;
   int return_code;

   if(argc < 2){
      fprintf(stderr, "Usage: %s <elf-file>\n", argv[0]);
      return EXIT_FAILURE;
   }

   elf = tuim_loader(argv[1]);
   if(tuim_error != TUIM_NO_ERROR){
      if(tuim_error == TUIM_ERROR_MEMORY)
         fprintf(stderr, "%s: Unsuficient memory.\n", tuim_error_filename);
      else if(tuim_error == TUIM_ERROR_READING)
         fprintf(stderr, "%s: Error reading the file.\n", tuim_error_filename);
      else if(tuim_error == TUIM_ERROR_INVALIDELF)
         fprintf(stderr, "%s: Not a valid ELF file.\n", tuim_error_filename);
      else if(tuim_error == TUIM_ERROR_MACHINE)
         fprintf(stderr, "%s: The ELF image is not for this machine.\n", tuim_error_filename);
      else if(tuim_error == TUIM_ERROR_SYSTEM)
         fprintf(stderr, "%s: The ELF image is not for this machine.\n", tuim_error_filename);
      else if(tuim_error == TUIM_ERROR_NOTDYN)
         fprintf(stderr, "%s: The ELF image is not a shared object.\n", tuim_error_filename);
      else if(tuim_error == TUIM_ERROR_MACHINE)
         fprintf(stderr, "%s: The ELF image is not for this machine.\n", tuim_error_filename);
      else if(tuim_error == TUIM_ERROR_UNSUPPORTED_RT)
         fprintf(stderr, "%s: Unsupported relocation type.\n", tuim_error_filename);
      return EXIT_FAILURE;
   }

   main_ = tuim_getentry(elf);
   if(main_){
      return_code = ((int(*)(int,char**))main_)(--argc, ++argv);
   }else{
      fprintf(stderr, "%s: Can't find the entry point.\n", argv[1]);
      return_code = EXIT_FAILURE;
   }

   tuim_free(elf);

   return return_code;
}
