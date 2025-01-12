#include <stddef.h>
#include <stdio.h>
#include "include/tuim.h"
#include "tuim_private.h"

int tuim_printelf(const tuim_elf *elf, const char *name){
   FILE *output;
   int return_value;

   output = fopen(name, "wb");
   if(output == NULL){
      tuim_error = TUIM_ERROR_READING;
      return -1;
   }
   return_value = fwrite(
      elf->program, 
      sizeof(unsigned char),
      (size_t)(elf->start_vaddr),
      output
   );
   fclose(output);

   if(return_value < 0)
      tuim_error = TUIM_ERROR_READING;

   return return_value;
}