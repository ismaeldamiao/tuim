#ifndef TUIM_H
#define TUIM_H

/* ---
Error codes
--- */
extern unsigned int tuim_error;
extern char *tuim_error_filename;
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

/* ---
Typedefs
--- */

typedef struct tuim_elf tuim_elf;

/* ---
Prototypes
--- */

/* Load a ELF file and its dependencies in memory */
tuim_elf* tuim_loader(const char *file_path);

/* Get the entry point of the program */
void* tuim_getentry(const tuim_elf *elf);

/* Find symbols on the program */
void* tuim_findsymbol(const char *symbol, const tuim_elf *elf);

/* Free the memory */
void tuim_free(tuim_elf *elf);

#endif /* TUIM_H */
