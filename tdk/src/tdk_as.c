/*
cd src/C
clang -std=c23 -o as as.c tdk_as.c tdk.c
cd ../../.bak
../src/C/as --target arm -o hellow_world.o hellow_world.asm
cd ..
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "tdk.h"

#include "tdk_utils.h"

struct tdk_as_args {
   char
      *output,
      *target,
      **input;
};

tdk_as_args_t* tdk_as_args_defaults(void){
   tdk_as_args_t *args;

   args = malloc(sizeof(tdk_as_args_t));
   if(args != NULL){
      args->output = NULL;

      args->target = malloc(sizeof(char) * (size_t)10);
      if(args->target != NULL){
         tdk_GetDefaultTarget(args->target);
      }else{
         free(args);
         return NULL;
      }

      args->input = malloc(sizeof(char*));
      if(args->input != NULL){
         args->input[0] = NULL;
      }else{
         free(args);
         return NULL;
      }
   }

   return args;
}

void tdk_as_args_free(tdk_as_args_t *args){
   if(args == NULL) return;
   FreePtrArray(args->input);
   free(args->target);
   free(args->output);
   free(args);
}

void tdk_as_args_SetTarget(tdk_as_args_t *args,char *target){
   args->target = ReallocString(args->target, target);
}

void tdk_as_args_SetOutput(tdk_as_args_t *args, char *output){
   args->output = ReallocString(args->output, output);
}

void tdk_as_args_AddInput(tdk_as_args_t *args, char *input){
   args->input = GrowStringArray(args->input, input);
}

#define STRCMP(a, b) (strcmp(a, b) == 0)

int tdk_as(tdk_as_args_t *args){
   char cmd[2048], **str;

   sprintf(
      cmd, 
      "clang -c -fPIC -x assembler-with-cpp"
   );

   {
      int i = 0;
      while(!STRCMP(args->target, target[i])){
         if(i == sizeof(target) / sizeof(char*)){
            fprintf(stderr, "cc: Unknown target.\n");
            return EXIT_FAILURE;
         }
         ++i;
      }
      sprintf(cmd, "%s --target=%s", cmd, triple[i]);
   }

   if(args->output != NULL){
      sprintf(cmd, "%s -o %s", cmd, args->output);
   }

   str = args->input;
   while(*str != NULL){
      sprintf(cmd, "%s %s", cmd, *str);
      ++str;
   }

   system(cmd);

   return EXIT_SUCCESS;
}
