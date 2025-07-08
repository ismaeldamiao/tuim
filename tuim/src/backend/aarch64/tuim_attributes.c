#include "../elf.h"
#include "../tuim_backend.h"

bool tuim_attributes(uint8_t *obj){
   /* Arm Architecture -- Little Endian
      arch64-unknown-none-elf */
   Elf64_Ehdr *ehdr = (void*)obj;
   Elf64_Word flags;
   if(obj[EI_DATA] != ELFDATA2LSB) return false;
   if(obj[EI_CLASS] != ELFCLASS64) return false;
   if(swap16(ehdr->e_machine) != EM_AARCH64) return false;
   return true;
}
