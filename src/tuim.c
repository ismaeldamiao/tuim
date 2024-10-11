/*
cmake -S tuim -B tuim/build
cmake --build tuim/build
cc -o tuim.elf tuim.c -L ./tuim/build -ltuim
*/

// Front-end to use tuim from the command line interface
#include <stdio.h>
#include <stdlib.h>

#include "tuim/tuim.h"
#include "crt/crt.h"

unsigned int status;

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
         fputs("Unsuficient memory.\n", stderr);
      else if(tuim_error == TUIM_ERROR_READING)
         fputs("Error reading the file.\n", stderr);
      else if(tuim_error == TUIM_ERROR_INVALIDELF)
         fputs("Not a valid ELF file.\n", stderr);
      else if(tuim_error == TUIM_ERROR_MACHINE)
         fputs("The ELF image is not for this machine.\n", stderr);
      else if(tuim_error == TUIM_ERROR_SYSTEM)
         fputs("The ELF image is not for this machine.\n", stderr);
      else if(tuim_error == TUIM_ERROR_NOTDYN)
         fputs("The ELF image is not a shared object.\n", stderr);
      else if(tuim_error == TUIM_ERROR_MACHINE)
         fputs("The ELF image is not for this machine.\n", stderr);
      else if(tuim_error == TUIM_ERROR_UNSUPPORTED_RT)
         fputs("Unsupported relocation type.\n", stderr);
      return EXIT_FAILURE;
   }

   main_ = tuim_getentry(elf);
   if(main_){
      //main_ = crt(main_, --argc, ++argv, &return_code);
      return_code = EXIT_SUCCESS;
      //main_();
      return_code = ((int(*)(void))main_)();
   }else{
      fputs("Can't find the 'main' function.\n", stderr);
      return_code = EXIT_FAILURE;
   }

   tuim_free(elf);

   return return_code;
}
