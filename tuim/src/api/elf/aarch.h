#ifndef _ELF_AARCH_H
#define _ELF_AARCH_H

#include "elf.h"
#if __STDC_VERSION__ < 202311L
#define true 1
#define false 0
typedef int bool;
#endif /* __STDC_VERSION__ < 202311L */

/* The Elf_class_native function returns true if the value of EI_CLASS
   is supported by the native execution environment, else returns false. */
static inline bool Elf_class_native(Elf_Byte *elf_buf){
   Elf_Byte *e_ident = elf_buf;
   #if __ARM_ARCH > 7
      return ((e_ident[EI_CLASS]==ELFCLASS32)||(e_ident[EI_CLASS]==ELFCLASS64));
   #else
      return (e_ident[EI_CLASS] == ELFCLASS32);
   #endif
}

#endif /* _ELF_AARCH_H */
