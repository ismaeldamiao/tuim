#ifndef TDK_H
#define TDK_H 1

void *tdk_GetDefaultTarget(char*);

/* Interface for the linker */
typedef struct tdk_ld_args tdk_ld_args_t;

tdk_ld_args_t* tdk_ld_args_defaults(void);
void tdk_ld_args_free(tdk_ld_args_t*);

void tdk_ld_args_EnableShared(tdk_ld_args_t*);
void tdk_ld_args_SetEntry(tdk_ld_args_t*,char*);
void tdk_ld_args_SetOutput(tdk_ld_args_t*,char*);
void tdk_ld_args_LinkLibrary(tdk_ld_args_t*,char*);
void tdk_ld_args_AddInput(tdk_ld_args_t*,char*);

int tdk_ld(tdk_ld_args_t*);

/* Interface for the assembler */
typedef struct tdk_as_args tdk_as_args_t;

tdk_as_args_t* tdk_as_args_defaults(void);
void tdk_as_args_free(tdk_as_args_t*);

void tdk_as_args_SetTarget(tdk_as_args_t*,char*);
void tdk_as_args_SetOutput(tdk_as_args_t*,char*);
void tdk_as_args_AddInput(tdk_as_args_t*,char*);

int tdk_as(tdk_as_args_t*);

/* Interface for the C compiler */
typedef struct tdk_cc_args tdk_cc_args_t;

tdk_cc_args_t* tdk_cc_args_defaults(void);
void tdk_cc_args_free(tdk_cc_args_t*);

void tdk_cc_args_SetTarget(tdk_cc_args_t*,char*);
void tdk_cc_args_SetOutput(tdk_cc_args_t*,char*);
void tdk_cc_args_AddInput(tdk_cc_args_t*,char*);

int tdk_cc(tdk_cc_args_t*);

/* Interface for the build system */
typedef struct tdk_mk_args tdk_mk_args_t;

tdk_mk_args_t* tdk_mk_args_defaults(void);
void tdk_mk_args_free(tdk_mk_args_t*);
void tdk_mk_args_SetTarget(tdk_mk_args_t*,char*);
void tdk_mk_args_SetInput(tdk_mk_args_t*,char*);

int tdk_mk(tdk_mk_args_t*);

#endif // TDK_H
