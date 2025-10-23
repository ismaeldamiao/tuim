/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 09, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"
#include <stdlib.h>

static int exec_init(tuim_ctx *ctx, tuim_elf *elf, char *const argv[], char *const envp[]){
   (void)ctx;
   (void)elf;
   (void)argv;
   (void)envp;
   return TUIM_SUCCESS;
}
static int exec_fini(tuim_ctx *ctx, tuim_elf *elf){
   (void)ctx;
   (void)elf;
   return TUIM_SUCCESS;
}

/*******************************************************************************
 * Function to execute...
 ******************************************************************************/
static int tuim_exec(tuim_ctx *ctx, const char *exec_path, char *const argv[], char *const envp[]){
   tuim_elf *elf;
   uintptr_t entry;
   int status, exit_code;

   is_that_tmp = (void*)exec_path;
   elf = list_find(ctx->elf_list, is_that);

   status = exec_init(ctx, elf, argv, envp);
   if(status != TUIM_SUCCESS) return status;

   entry = Elf32_symbol(elf, NULL);

   status = tuim_jump(ctx, entry);
   if(status != TUIM_SUCCESS) return status;

   status = exec_fini(ctx, elf);
   if(status != TUIM_SUCCESS) return status;

   return TUIM_SUCCESS;
}
