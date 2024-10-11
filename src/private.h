#ifndef TUIM_PRIVATE_H
#define TUIM_PRIVATE_H

#include <stdio.h>

#define SUCCESS          0x0000
#define ERROR_OPENFILE   0x0001
#define ERROR_INVALIDELF 0x0002

// TODO: Improve error handling
extern unsigned int status;

typedef void(*entry_t)(void);
typedef struct elf_s {
   void *ehdr;
   void *phdr;
   void *shdr;
   void *symtab;
   void **pdata;
   void **sdata;
   /**/
   FILE *file;
   const char *file_path;
   void *entry_point;
} elf_t;

elf_t* load(const char *file_path);
void relocate(elf_t *elf);
void exec(elf_t *elf); //???
int cmain(int argc, char **argv);

unsigned int fread_(void *buf, uint32_t bytes, uint32_t offset, FILE *fil);

#endif // TUIM_PRIVATE_H
