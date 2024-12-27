#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdk.h"

#define STRCMP(a, b) (strcmp(a, b) == 0)

int main(int argc, char **argv){
   unsigned int tdk_version, release_major_version,  release_minor_version;
   char version_str[512], default_target[10];
   tdk_ld_args_t *args;

   if(argc < 2) return 1;

   args = tdk_ld_args_defaults();
   if(args == NULL){
      fputs("Error: Out of memory.", stderr);
      return EXIT_FAILURE;
   }

   /* The version */
   tdk_version = 0U;
   release_major_version = 0U;
   release_minor_version = 1U;
   tdk_GetDefaultTarget(default_target);
   strcpy(version_str,
      "TDK version %u\n"
      "Copyright (C) 2024 I.F.F. dos Santos <ismaellxd@gmail.com>\n"
      "|-- version %u.%u\n"
      "`-- configured for the target '%s'\n"
   );

   /* Parse arguments */
   for(int i = 1; i < argc; ++i){
      if(argv[i][0] == '-'){
         if(STRCMP(argv[i], "--version")){
            printf(
               version_str,
               tdk_version,
               release_major_version,
               release_minor_version,
               default_target
            );
            return EXIT_SUCCESS;
         }else if(STRCMP(argv[i], "--shared")){
            tdk_ld_args_EnableShared(args);
         }else if(STRCMP(argv[i], "-e") || STRCMP(argv[i], "--entry")){
            tdk_ld_args_SetEntry(args, argv[++i]);
         }else if(argv[i][1] == 'l'){
            tdk_ld_args_LinkLibrary(args, argv[i] + 2);
         }else if(STRCMP(argv[i], "-o")){
            tdk_ld_args_SetOutput(args, argv[++i]);
         }else{
            fprintf(stderr, "Error: Unknown option %s.\n", argv[i]);
            return EXIT_FAILURE;
         }
      }else{
         tdk_ld_args_AddInput(args, argv[i]);
      }
   }

   tdk_ld(args);
   tdk_ld_args_free(args);

   return EXIT_SUCCESS;
}
