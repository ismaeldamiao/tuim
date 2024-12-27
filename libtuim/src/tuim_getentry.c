#include <stddef.h>
#include <stdint.h>

#include "include/tuim.h"
#include "elf.h"
#include "tuim_private.h"

void* tuim_getentry(const tuim_elf *elf){
   uintptr_t e_entry;
   Elf_Ehdr *ehdr;

   if(elf == NULL) return NULL;

   ehdr = elf->ehdr;
   e_entry = ELF_E_ENTRY(*ehdr);

   return (uint8_t*)(elf->program) + e_entry - elf->start_vaddr;
}
