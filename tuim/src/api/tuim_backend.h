#ifndef TUIM_BACKEND_HOST_H
#define TUIM_BACKEND_HOST_H

#include <stdint.h>
#include "tuim.h"

#if __STDC_VERSION__ < 202311L
   #define true 1
   #define false 0
#endif
#if __STDC_VERSION__ < 199901L
   typedef int bool;
#elif (__STDC_VERSION__ >= 199901L) && (__STDC_VERSION__ < 202311L)
   typedef _Bool bool;
#endif

bool     tuim_attributes(uint8_t *obj);
void    *tuim_get_section(const uint8_t *obj, uint8_t *section);

/* note that all pointer on the following structure shall point to objects
   on the host machine's memory */
struct tuim_backend {
   /* string holding the content of a ELF file */
   const uint8_t *obj;

   /* dynamic section */
   void *dyn;
   uint8_t *dynstr;

   /* dynamic rellocation tables */
   void *rel, *rela;
   size_t relent, relaent;
   size_t relsz, relasz;
   size_t relnum, relanum;

   void *auxiliary;

   /* others informations */
   void *program_image;
   size_t program_size;
   uint64_t start_vaddr;
};

int      tuim_load_segments (const tuim_ctx *ctx, const uint8_t *obj, void **ptr);
uint8_t* tuim_get_dependency(const tuim_ctx *ctx, void *ptr);
void   * tuim_get_relocation(const tuim_ctx *ctx, void *ptr);
uint8_t* tuim_relocate(const tuim_ctx *ctx, void *ptr, void *rel, void *dep);
uint64_t tuim_get_symbol(const tuim_ctx *ctx, void *ptr, uint8_t *symbol);
void     tuim_jump(const tuim_ctx *ctx, uint64_t address);

#endif /* TUIM_BACKEND_H */
