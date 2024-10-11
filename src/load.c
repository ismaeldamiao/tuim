#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>

#include "elf.h"
#include "malloc/malloc.h"
#include "private.h"

static void load32(elf_t *elf){
   Elf32_Ehdr *ehdr = NULL;
   Elf32_Phdr *phdr = NULL;
   Elf32_Shdr *shdr = NULL;
   uint16_t ehsize = UINT16_C(52);
   uint16_t phentsize = UINT16_C(32);
   uint16_t shentsize = UINT16_C(40);
   void *buf;
   uint32_t bytes, offset;
   FILE *fil;

   ehdr = malloc_((size_t)ehsize, MALLOC_READ | MALLOC_WRITE);
   if(!ehdr){
      /* TODO: Handle error */
      return;
   }
   ehdr->e_ident[EI_MAG0] = ((uint8_t*)(elf->ehdr))[EI_MAG0];
   ehdr->e_ident[EI_MAG1] = ((uint8_t*)(elf->ehdr))[EI_MAG1];
   ehdr->e_ident[EI_MAG2] = ((uint8_t*)(elf->ehdr))[EI_MAG2];
   ehdr->e_ident[EI_MAG3] = ((uint8_t*)(elf->ehdr))[EI_MAG3];
   ehdr->e_ident[EI_CLASS] = ((uint8_t*)(elf->ehdr))[EI_CLASS];
   ehdr->e_ident[EI_DATA] = ((uint8_t*)(elf->ehdr))[EI_DATA];
   ehdr->e_ident[EI_VERSION] = ((uint8_t*)(elf->ehdr))[EI_VERSION];
   ehdr->e_ident[EI_OSABI] = ((uint8_t*)(elf->ehdr))[EI_OSABI];
   ehdr->e_ident[EI_ABIVERSION] = ((uint8_t*)(elf->ehdr))[EI_ABIVERSION];

   buf = (uint8_t*)ehdr + EI_NIDENT;
   bytes = (uint32_t)(ehsize - EI_NIDENT);
   offset = (uint32_t)EI_NIDENT;
   fil = elf->file;
   status = fread_(buf, bytes, offset, fil);
   if(status != SUCCESS) return;

   /* Load the program header table */
   if(ehdr->e_phnum != UINT16_C(0)){
      if(ehdr->e_phentsize != phentsize){
         /* TODO: Handle error */
         return;
      }
      bytes = (uint32_t)(ehdr->e_phentsize * ehdr->e_phnum);
      phdr = malloc_(bytes, MALLOC_WRITE);
      if(!phdr){
         /* TODO: Handle error */
         return;
      }
      buf = phdr;
      offset = (uint32_t)(ehdr->e_phoff);
      status = fread_(buf, bytes, offset, fil);
      if(status != SUCCESS) return;
   }

   /* Load the section header table */
   if(ehdr->e_shnum != UINT16_C(0)){
      if(ehdr->e_shentsize != shentsize){
         /* TODO: Handle error */
         return;
      }
      bytes = (uint32_t)(ehdr->e_shentsize * ehdr->e_shnum);
      shdr = malloc_(bytes, MALLOC_WRITE);
      if(!shdr){
         /* TODO: Handle error */
         return;
      }
      buf = shdr;
      offset = (uint32_t)(ehdr->e_shoff);
      status = fread_(buf, bytes, offset, fil);
      if(status != SUCCESS) return;
   }

   /* Load segments */
   if(phdr){
      uint16_t i;
      bytes = ((uint32_t)sizeof(void*)) * (uint32_t)ehdr->e_phnum;
      elf->pdata = malloc_(bytes, MALLOC_WRITE);
      if(!(elf->pdata)){
         /* TODO: Handle error */
      }
      for(i = UINT16_C(0); i < ehdr->e_phnum; ++i){
         if(phdr[i].p_type == PT_NULL){
            elf->pdata[i] = NULL;
         }else if(phdr[i].p_type == PT_LOAD){
            if(phdr->p_memsz == UINT32_C(0)){
               elf->pdata[i] = NULL;
               continue;
            }
            if(phdr->p_flags & PF_X){
               elf->pdata[i] = malloc_(phdr->p_memsz, MALLOC_EXEC);
            }else if(phdr->p_flags & PF_W){
               elf->pdata[i] = malloc_(phdr->p_memsz, MALLOC_WRITE);
            }
            if(!(elf->pdata[i])){
               /* TODO: Handle error */
               continue;
            }
            buf = elf->pdata[i];
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->pdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr[i].p_memsz - phdr[i].p_filesz;
            memset(elf->pdata[i] + phdr[i].p_filesz, 0, (size_t)bytes);
         }else if(phdr[i].p_type == PT_DYNAMIC){
            elf->pdata[i] = malloc_(phdr->p_memsz, MALLOC_WRITE);
            if(!(elf->pdata[i])){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->pdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
         }else if(phdr[i].p_type == PT_INTERP){
            elf->pdata[i] = NULL;
         }else if(phdr[i].p_type == PT_NOTE){
            elf->pdata[i] = NULL;
         }else if(phdr[i].p_type == PT_SHLIB){
            elf->pdata[i] = malloc_(phdr->p_memsz, MALLOC_READ | MALLOC_WRITE);
            if(!(elf->pdata[i])){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->pdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
         }else{
            elf->pdata[i] = NULL;
         }
      }
   }

   /* Load section */
   if(shdr){
      uint16_t i;
      elf->sdata = malloc(sizeof(void*) * (size_t)(ehdr->e_shnum));
      if(!(elf->sdata)){
         /* TODO: Handle error */
      }
      for(i = UINT16_C(0); i < ehdr->e_shnum; ++i){
         if(shdr[i].sh_type == SHT_NULL){
            elf->sdata[i] = NULL;
         }else if(shdr[i].sh_type == SHT_PROGBITS){
            elf->sdata[i] = malloc_(shdr->sh_size, MALLOC_WRITE);
            if(elf->sdata[i] == NULL){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->sdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
         }else if(shdr[i].sh_type == SHT_SYMTAB){
            elf->sdata[i] = malloc_(shdr->sh_size, MALLOC_WRITE);
            if(elf->sdata[i] == NULL){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->sdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
         }else if(shdr[i].sh_type == SHT_STRTAB){
            elf->sdata[i] = malloc_(shdr->sh_size, MALLOC_WRITE);
            if(elf->sdata[i] == NULL){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->sdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
         }else if(shdr[i].sh_type == SHT_RELA){
            elf->sdata[i] = malloc_(shdr->sh_size, MALLOC_WRITE);
            if(elf->sdata[i] == NULL){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->sdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
         }else if(shdr[i].sh_type == SHT_HASH){
            elf->sdata[i] = malloc_(shdr->sh_size, MALLOC_WRITE);
            if(elf->sdata[i] == NULL){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->sdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
         }else if(shdr[i].sh_type == SHT_DYNAMIC){
            elf->sdata[i] = malloc_(shdr->sh_size, MALLOC_WRITE);
            if(elf->sdata[i] == NULL){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->sdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
         }else if(shdr[i].sh_type == SHT_NOTE){
            elf->sdata[i] = NULL;
         }else if(shdr[i].sh_type == SHT_NOBITS){
            elf->sdata[i] = NULL;
         }else if(shdr[i].sh_type == SHT_REL){
            elf->sdata[i] = malloc_(shdr->sh_size, MALLOC_WRITE);
            if(elf->sdata[i] == NULL){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->sdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
         }else if(shdr[i].sh_type == SHT_HASH){
            elf->sdata[i] = malloc_(shdr->sh_size, MALLOC_WRITE);
            if(elf->sdata[i] == NULL){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->sdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
         }else if(shdr[i].sh_type == SHT_DYNAMIC){
            elf->sdata[i] = malloc_(shdr->sh_size, MALLOC_WRITE);
            if(elf->sdata[i] == NULL){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->sdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
         }else if(shdr[i].sh_type == SHT_SHLIB){
            elf->sdata[i] = NULL;
         }else if(shdr[i].sh_type == SHT_DYNSYM){
            elf->sdata[i] = malloc_(shdr->sh_size, MALLOC_WRITE);
            if(elf->sdata[i] == NULL){
               /* TODO: Handle error */
               continue;
            }
            bytes = phdr->p_memsz;
            offset = phdr->p_offset;
            status = fread_(elf->sdata[i], bytes, offset, elf->file);
            if(status != SUCCESS){
               /* TODO: Handle error */
               continue;
            }
         }else{
            elf->sdata[i] = NULL;
         }
      }
   }

   elf->ehdr = ehdr;
   elf->phdr = phdr;
   elf->shdr = shdr;
   return;
}

static void load64(elf_t *elf){
   Elf64_Ehdr *ehdr = NULL;
   Elf64_Phdr *phdr = NULL;
   Elf64_Shdr *shdr = NULL;
   void *buf;
   uint32_t bytes, offset;
   FILE *fil;

   ehdr = malloc_(sizeof(*shdr), MALLOC_WRITE);
   if(!ehdr){
      /* TODO: Handle error */
      return;
   }
   ehdr->e_ident[EI_MAG0] = ((uint8_t*)(elf->ehdr))[EI_MAG0];
   ehdr->e_ident[EI_MAG1] = ((uint8_t*)(elf->ehdr))[EI_MAG1];
   ehdr->e_ident[EI_MAG2] = ((uint8_t*)(elf->ehdr))[EI_MAG2];
   ehdr->e_ident[EI_MAG3] = ((uint8_t*)(elf->ehdr))[EI_MAG3];
   ehdr->e_ident[EI_CLASS] = ((uint8_t*)(elf->ehdr))[EI_CLASS];
   ehdr->e_ident[EI_DATA] = ((uint8_t*)(elf->ehdr))[EI_DATA];
   ehdr->e_ident[EI_VERSION] = ((uint8_t*)(elf->ehdr))[EI_VERSION];
   ehdr->e_ident[EI_OSABI] = ((uint8_t*)(elf->ehdr))[EI_OSABI];
   ehdr->e_ident[EI_ABIVERSION] = ((uint8_t*)(elf->ehdr))[EI_ABIVERSION];

   buf = (uint8_t*)ehdr + EI_NIDENT;
   bytes = UINT32_C(64) - (uint32_t)EI_NIDENT;
   offset = (uint32_t)EI_NIDENT;
   fil = elf->file;
   status = fread_(buf, bytes, offset, fil);
   if(status != SUCCESS) return;

   /* Load the program header table */
   if(ehdr->e_phnum != UINT16_C(0)){
      if(ehdr->e_shentsize != UINT16_C(56)){
         /* TODO: Handle error */
         return;
      }
      bytes = (uint32_t)(ehdr->e_phentsize * ehdr->e_phnum);
      phdr = malloc_(bytes, MALLOC_WRITE);
      if(!phdr){
         /* TODO: Handle error */
         return;
      }
      buf = phdr;
      offset = (uint32_t)(ehdr->e_phoff);
      status = fread_(buf, bytes, offset, fil);
      if(status != SUCCESS) return;
   }

   /* Load the section header table */
   if(ehdr->e_shnum != UINT16_C(0)){
      if(ehdr->e_shentsize != UINT16_C(64)){
         /* TODO: Handle error */
         return;
      }
      bytes = (uint32_t)(ehdr->e_shentsize * ehdr->e_shnum);
      shdr = malloc_(bytes, MALLOC_WRITE);
      if(!shdr){
         /* TODO: Handle error */
         return;
      }
      buf = shdr;
      offset = (uint32_t)(ehdr->e_shoff);
      status = fread_(buf, bytes, offset, fil);
      if(status != SUCCESS) return;
   }

   return;
}

elf_t* load(const char *file_path){
   elf_t *elf;
   FILE *file_ptr;

#if 0
   {
      /* Do not try to load a fila that already is loaded/loading */
      struct xor_list *tmp_list, *prev, *next;
      tmp_list = started_elf->first;
      prev = NULL;
      while(tmp_list){ /* Transverse the list of loaded/loading ELFs */
         ld_elf *tmp_elf;
         tmp_elf = tmp_list->data;
         if(strcmp(file_path, tmp_elf->file_path) != 0) return NULL;
         next = XOR(prev, tmp_list->link);
         prev = tmp_list;
         tmp_list = next;
      }
   }
#endif

   file_ptr = fopen(file_path, "rb");
   if(file_ptr == NULL){
      // TODO: Set status
      return NULL;
   }

   elf = malloc(sizeof(elf_t));
   if(elf == NULL){
      // TODO: Set status
      return NULL;
   }
   elf->file = file_ptr;
   elf->file_path = file_path;

   /* ----------
   Stage 0: Load the ELF header, program table and section table.
   ---------- */

   /* Load the ELF header independent code */
   {
      uint8_t *e_ident = NULL;

      e_ident = malloc_((size_t)EI_NIDENT, MALLOC_WRITE);
      if(e_ident == NULL){
         // TODO: Set status
         return NULL;
      }

      status = fread_(&e_ident,(uint32_t)EI_NIDENT,UINT32_C(0),file_ptr);
      if(status != SUCCESS)
         return NULL;
      else if(
         (e_ident[EI_MAG0] != ELFMAG0) ||
         (e_ident[EI_MAG1] != ELFMAG1) ||
         (e_ident[EI_MAG2] != ELFMAG2) ||
         (e_ident[EI_MAG3] != ELFMAG3)
      ){
         // TODO: Set status
         return NULL;
      }else if(e_ident[EI_VERSION] != EV_CURRENT){
         // TODO: Set status
         return NULL;
      }

      elf->ehdr = e_ident;

      if(e_ident[EI_CLASS] == ELFCLASS32)
         load32(elf);
      else if(e_ident[EI_CLASS] == ELFCLASS64)
         load64(elf);

      free(e_ident);
   }

   fclose(elf->file);
   return elf;
}
