#ifndef TUIM_H
#define TUIM_H

#ifdef __cplusplus
extern "C" {
#endif
/* ------------------------------------
   Public header file for the Tuim project.
   * Last modified: January 13, 2025.
------------------------------------ */

/* Version */
#define TUIM_VERSION_MAJOR 0
#define TUIM_VERSION_MINOR 1

/* ---
Error codes
--- */
void tuim_perror(const char *s);
char* tuim_strerror(unsigned int errnum);
extern unsigned int tuim_errno;
#define TUIM_NO_ERROR             0x0000U
#define TUIM_EMEMORY         0x0001U
#define TUIM_EREADING        0x0002U
#define TUIM_EINVALIDELF     0x0003U
#define TUIM_EMACHINE        0x0004U
#define TUIM_ESYSTEM         0x0005U
#define TUIM_ENOTDYN         0x0006U
#define TUIM_EUNSUPPORTED_RT 0x0007U
#define TUIM_ESYMBOLNOTFOUND 0x0008U
#define TUIM_EBADMEMLAYOUT   0x0009U
#define TUIM_EENTRY          0x000AU
#define TUIM_EUNKNOWNTYPE    0x000BU
#define TUIM_EPROC           0x000CU
#define TUIM_ENOTEXEC        0x000DU

extern char *tuim_error_filename;
extern char *tuim_str;

/* ---
Typedefs
--- */

typedef struct tuim_elf tuim_elf;

/* ---
Prototypes
--- */

/* Get the default target */
char* tuim_target(void);

/* Load a ELF file and its dependencies in memory */
tuim_elf* tuim_loader(const char *file_path, unsigned flags);
#define TUIM_LOADER_FNONE 0U
#define TUIM_LOADER_FEXEC (1U << 0)
#define TUIM_LOADER_FDYN  (2U << 1)
#define TUIM_LOADER_FREL  (2U << 2)

/* Get the entry point of the program */
void* tuim_getentry(const tuim_elf *elf);

/* Execute a executable ELF */
void* tuim_exec(unsigned type, unsigned flags, tuim_elf *elf, void *args);
#define TUIM_EXEC_TNONE 0U
#define TUIM_EXEC_TC    1U
#define TUIM_EXEC_FNONE 0U

/* Find symbols on the program */
void* tuim_findsymbol(const char *symbol, const tuim_elf *elf);

/* Free the memory */
void tuim_free(tuim_elf *elf);

#ifdef __cplusplus
}
#endif

#endif /* TUIM_H */
