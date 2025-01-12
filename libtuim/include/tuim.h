#ifndef TUIM_H
#define TUIM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Version */
#define TUIM_VERSION_MAJOR 0
#define TUIM_VERSION_MINOR 1

/* ---
Error codes
--- */
extern unsigned int tuim_error;
extern char *tuim_error_filename;
extern char *tuim_str;
#define TUIM_NO_ERROR             0x0000U
#define TUIM_ERROR_MEMORY         0x0001U
#define TUIM_ERROR_READING        0x0002U
#define TUIM_ERROR_INVALIDELF     0x0003U
#define TUIM_ERROR_MACHINE        0x0004U
#define TUIM_ERROR_SYSTEM         0x0005U
#define TUIM_ERROR_NOTDYN         0x0006U
#define TUIM_ERROR_UNSUPPORTED_RT 0x0007U
#define TUIM_ERROR_SYMBOLNOTFOUND 0x0008U
#define TUIM_ERROR_BADMEMLAYOUT   0x0009U
void tuim_perror(const char *s);
char* tuim_strerror(unsigned int errnum);

/* ---
Typedefs
--- */

typedef struct tuim_elf tuim_elf;
typedef struct tuim_source tuim_source;
typedef struct tuim_project tuim_project;

/* ---
Prototypes
--- */

/* Get the default target */
char* tuim_target(void);

/* Load a ELF file and its dependencies in memory */
tuim_elf* tuim_loader(const char *file_path);

/* Get the entry point of the program */
void* tuim_getentry(const tuim_elf *elf);

/* Find symbols on the program */
void* tuim_findsymbol(const char *symbol, const tuim_elf *elf);

/* Free the memory */
void tuim_free(tuim_elf *elf);


#define TUIM_LD_FILE 0x00000001U
#define TUIM_LD_ELF  0x00000002U
#define TUIM_LD_SHARED 0x00000004U
tuim_elf* tuim_ld(void *input, const char *entry, char **libs, uint64_t args);

#define TUIM_AS_FILE 0x00000001U
#define TUIM_AS_STR  0x00000002U
tuim_elf* tuim_as(void *input, const char *target, const char *include, uint64_t args);

#define TUIM_CC_FILE 0x00000001U
#define TUIM_CC_STR  0x00000002U
tuim_elf* tuim_cc(void *input, const char *target, const char *include, uint64_t args);

int tuim_printelf(const tuim_elf *elf, const char *name);

#ifdef __cplusplus
}
#endif

#endif /* TUIM_H */
