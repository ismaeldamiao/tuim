#include <stdint.h>
#include <tuim/arch.h>
#include "elf.h"


#if   tuim_arch == tuim_riscv32
   #error Support to RISC-V architecture is not implemented yet
#elif tuim_arch == tuim_riscv64
   #error Support to RISC-V architecture is not implemented yet
#elif tuim_arch == tuim_arm
/* ------------------------------------
   Attributes for armel-unknown-unknown-eabi target.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
int tuim_check_attributes(uint8_t *obj){
   Elf32_Ehdr *ehdr = (void*)obj;
   Elf32_Word flags = ehdr->e_flags;;
   if(obj[EI_DATA] != ELFDATA2LSB) return 1;
   if(obj[EI_CLASS] != ELFCLASS32) return 1;
   if(ehdr->e_machine != EM_ARM) return 1;
   if((flags & EF_ARM_EABI_VER5) == 0) return 1;
   #if __ARM_ARCH < 7
      /* armv7l-unknown-unknown-eabi */
      if((flags & EF_ARM_ABI_FLOAT_SOFT) == 0) return 1;
   #endif
   return 0;
}
#elif tuim_arch == tuim_aarch64
/* ------------------------------------
   Attributes for arch64-unknown-unknown-elf target.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
int tuim_check_attributes(uint8_t *obj){
   Elf64_Ehdr *ehdr = (void*)obj;
   if(obj[EI_DATA] != ELFDATA2LSB) return 1;
   if(obj[EI_CLASS] != ELFCLASS64) return 1;
   if(ehdr->e_machine != EM_AARCH64) return 1;
   return 0;
}
#elif tuim_arch == tuim_i386
   #error Support to Intel 386 architecture is not implemented yet
#elif tuim_arch == tuim_amd64
/* ------------------------------------
   Attributes for x86_64-unknown-unknown-elf target.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
int tuim_check_attributes(uint8_t *obj){
   Elf32_Ehdr *ehdr = (void*)obj;
   if(obj[EI_DATA] != ELFDATA2LSB) return 1;
   if(obj[EI_CLASS] != ELFCLASS64) return 1;
   if(ehdr->e_machine != EM_X86_64) return 1;
   return 0;
}
#else
   #error Target Instruction Set Architecture is not suported
#endif
