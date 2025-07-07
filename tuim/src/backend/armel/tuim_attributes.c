#include "../../elf.h"

#ifndef swap16
   #define swap16(x) (x)
#endif

#ifndef swap32
   #define swap32(x) (x)
#endif

#ifndef swap64
   #define swap64(x) (x)
#endif

#ifndef Elf
   #define Elf(x) Elf32_##x
#endif

#if __STDC_VERSION__ < 202311L
   #define true 1
   #define false 0
   typedef int bool;
#endif /* __STDC_VERSION__ < 202311L */

bool tuim_attributes(uint8_t *obj){
   /* Arm Architecture -- Little Endian
      armel-unknown-none-eabi */
   Elf32_Ehdr *ehdr = (void*)obj;
   Elf32_Word flags;
   if(obj[EI_DATA] != ELFDATA2LSB) return false;
   if(obj[EI_CLASS] != ELFCLASS32) return false;
   if(swap16(ehdr->e_machine) != EM_ARM) return false;
   flags = swap32(ehdr->e_flags) & (
      EF_ARM_EABI_VER5 |
      EF_ARM_ABI_FLOAT_SOFT
   );
   return (bool)flags;
}
