#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "tdk.h"

#include "tdk_utils.h"

struct tdk_cc_args {
   char
      *output,
      *target,
      **input;
};

tdk_cc_args_t* tdk_cc_args_defaults(void){
   tdk_cc_args_t *args;

   args = malloc(sizeof(tdk_cc_args_t));
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

void tdk_cc_args_free(tdk_cc_args_t *args){
   if(args == NULL) return;
   FreePtrArray(args->input);
   free(args->target);
   free(args->output);
   free(args);
}

void tdk_cc_args_SetTarget(tdk_cc_args_t *args,char *target){
   args->target = ReallocString(args->target, target);
}

void tdk_cc_args_SetOutput(tdk_cc_args_t *args, char *output){
   args->output = ReallocString(args->output, output);
}

void tdk_cc_args_AddInput(tdk_cc_args_t *args, char *input){
   args->input = GrowStringArray(args->input, input);
}

#define STRCMP(a, b) (strcmp(a, b) == 0)

int tdk_cc(tdk_cc_args_t *args){
   char cmd[2048], *tuim_home, **str;

   sprintf(
      cmd, 
      "clang --std=c23 -c -fPIC -ffreestanding"
   );

   if(STRCMP(args->target, "arm")){
      sprintf(cmd, "%s --target=arm-none-eabi", cmd);
   }else if(STRCMP(args->target, "armv7a")){
      sprintf(cmd, "%s --target=armv7a-none-eabi", cmd);
   }else{
      return EXIT_FAILURE;
   }

   tuim_home = getenv("TUIM_HOME");
   if(tuim_home != NULL){
      sprintf(cmd, "%s -nostdinc -I '%s/include'", cmd, tuim_home);
   }

   if(args->output != NULL){
      sprintf(cmd, "%s -o %s", cmd, args->output);
   }

   str = args->input;
   while(*str != NULL){
      sprintf(cmd, "%s %s", cmd, *str);
      ++str;
   }

   //puts(cmd);
   system(cmd);

   return EXIT_SUCCESS;
}
