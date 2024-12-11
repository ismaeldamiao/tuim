#ifndef TUIM_PRIVATE_H
#define TUIM_PRIVATE_H 1

#include <stdint.h>

struct tuim_elf {
   void *ehdr;   /* Pointer to ELF header */
   void *phdr;   /* Program header array */
   void *shdr;   /* Section header array */
   void **segments; /* Array of pointers to segments */
   void **sections; /* Array of pointers to sections */
   void *program; /* Program memory image (value is the load address) */
   uintptr_t start_vaddr; /* Lowest virtual address of a PT_LOAD segment. */
   /* Note that the base address is: (uintptr_t)program - start_vaddr */
};

#endif /* TUIM_PRIVATE_H */
