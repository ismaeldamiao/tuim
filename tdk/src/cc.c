#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdk.h"

#define STRCMP(a, b) (strcmp(a, b) == 0)

int main(int argc, char **argv){
   unsigned int tdk_version, release_major_version,  release_minor_version;
   char version_str[512], default_target[10];
   tdk_cc_args_t *args;

   if(argc < 2) return 1;

   args = tdk_cc_args_defaults();

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
         }else if(STRCMP(argv[i], "--target")){
            tdk_cc_args_SetTarget(args, argv[++i]);
         }else if(STRCMP(argv[i], "-o")){
            tdk_cc_args_SetOutput(args, argv[++i]);
         }else{
            fprintf(stderr, "Error: Unknown option %s.\n", argv[i]);
            return EXIT_FAILURE;
         }
      }else{
         tdk_cc_args_AddInput(args, argv[i]);
      }
   }

   tdk_cc(args);
   tdk_cc_args_free(args);

   return EXIT_SUCCESS;
}
