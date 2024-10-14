#include <stdint.h>

#include "include/tuim.h"
#include "elf.h"
#include "tuim_utils.h"

void* tuim_getentry(const tuim_elf *elf){
   void* entry;
   uint16_t i;
   uintptr_t e_entry;
   Elf_Ehdr *ehdr;

   ehdr = elf->ehdr;

   e_entry = ELF_E_ENTRY(*ehdr);
   i = phndx(elf, e_entry);

   entry = segment_base_addr(elf, i) + e_entry;
   return entry;
}
