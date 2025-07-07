#ifndef TUIM_CTX_H
#define TUIM_CTX_H
#include <stdint.h>

/* ------------------------------------
   Definition of tuim_ctx_s strcture.
   * Part of Tuim Project.
   * Last modified: June 17, 2025.
------------------------------------ */

#define TUIM_CTX_MAX_LEN 512

#if __STDC_VERSION__ < 202311L
   #define true 1
   #define false 0
#endif
#if __STDC_VERSION__ < 199901L
   typedef int bool;
#elif (__STDC_VERSION__ >= 199901L) && (__STDC_VERSION__ < 202311L)
   typedef _Bool bool;
#endif

typedef struct tuim_ctx_s tuim_ctx;
struct tuim_ctx_s {
   /* environment variables */
   uint8_t tuim_home[TUIM_CTX_MAX_LEN], ld_library_path[TUIM_CTX_MAX_LEN];
   uint8_t *machine; /* machine */

   bool (*have_correct_attributes)(uint8_t*);
   int      (*load_segments) (const tuim_ctx *ctx, const uint8_t *obj, void **ptr);
   const uint8_t* (*get_dependency)(const tuim_ctx *ctx, void *ptr);
   const void *   (*get_relocation)(const tuim_ctx *ctx, void *ptr);
   const uint8_t* (*relocate)(const tuim_ctx *ctx, void *ptr, const void *rel, const void *dep);
   uint64_t (*get_symbol)(const tuim_ctx *ctx, void *ptr, const uint8_t *symbol);
   void     (*jump)(const tuim_ctx *ctx, uint64_t address);

   void * (*get_section)(const uint8_t*,uint8_t*);

   struct list *elf_list; /* list of ELFs */
   /* error handling */
   int status;
   char *strerror;
   bool can_load_exec;
};
#endif /* TUIM_CTX_H */
