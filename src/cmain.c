#include <stdint.h>
#include <stdlib.h>
#include "private.h"
#include "crt/crt.h"

int cmain(int argc, char **argv){
   void *jmp_addr;
   elf_t *elf;
   int ret;

   elf = load(argv[0]);
   if(elf == NULL){
      // TODO: Set ld_status
      return EXIT_FAILURE;
   }
   /* Relocate */
   relocate(elf);

   /* Get the entry point and resolve symbols */
   //entry = ld_GetEntry(file_ptr);

   /* Prepare the main() call */
   jmp_addr = crt(elf->entry_point, argc, argv, &ret);

   /* Jump to the entry point */
   {
      entry_t entry = jmp_addr;
      entry();
   }
   
   free(elf->ehdr);
   free(elf->phdr);
   free(elf->shdr);
   free(elf->symtab);
   free(elf->pdata);
   free(elf->sdata);
   free(elf->entry_point);
   free(elf);
   free(jmp_addr);
   return ret;
}

