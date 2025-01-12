#include "tuim.h"
#ifndef TUIM_PRIVATE_H
#define TUIM_PRIVATE_H 1

#include <stddef.h>
#include <stdint.h>



/* --------------------- */
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
/* --------------------- */

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

#include "elf.h"
#include <string.h>
#include <stdlib.h>

static char *get_triple(uint16_t e_machine){
   if(e_machine == EM_ARM)
      return "arm-none-eabi";
   return NULL;
}

static char *strcat_(char *dest, const char *src){
   char *buf;
   buf = realloc(dest, strlen(dest) + strlen(src) + (size_t)1);
   if(buf != NULL){
      strcat(buf, src);
   }
   return buf;
}

#endif /* TUIM_PRIVATE_H */
