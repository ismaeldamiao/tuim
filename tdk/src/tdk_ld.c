/*
cd src/C
clang -std=c23 -o ld ld.c tdk_ld.c tdk.c
cd ../../.bak
../src/C/ld -o hellow_world.elf -lc hellow_world.o
cd ..
*/
#include <stdio.h>
#include <stdlib.h>
#include "tdk.h"
#include "tdk.h"

#include "tdk_utils.h"

struct tdk_ld_args {
   char
      *entry,
      *output,
      **library,
      **input;
   bool shared;
};

tdk_ld_args_t* tdk_ld_args_defaults(void){
   tdk_ld_args_t *args;

   args = malloc(sizeof(tdk_ld_args_t));
   if(args != NULL){
      args->shared = false;
      args->output = NULL;

      args->entry = malloc(sizeof("main"));
      if(args->entry != NULL){
         strcpy(args->entry, "main");
      }else{
         free(args);
         return NULL;
      }

      args->library = malloc(sizeof(char*));
      if(args->library != NULL){
         args->library[0] = NULL;
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

void tdk_ld_args_free(tdk_ld_args_t *args){
   if(args == NULL) return;

   FreePtrArray(args->library);
   FreePtrArray(args->input);
   free(args->entry);
   free(args->output);
   free(args);
}

void tdk_ld_args_EnableShared(tdk_ld_args_t *args){
   args->shared = true;
}

void tdk_ld_args_SetEntry(tdk_ld_args_t *args, char *entry){
   args->entry = ReallocString(args->entry, entry);
}

void tdk_ld_args_SetOutput(tdk_ld_args_t *args, char *output){
   args->output = ReallocString(args->output, output);
}

void tdk_ld_args_LinkLibrary(tdk_ld_args_t *args, char *library){
   args->library = GrowStringArray(args->library, library);
}

void tdk_ld_args_AddInput(tdk_ld_args_t *args, char *input){
   args->input = GrowStringArray(args->input, input);
}

int tdk_ld(tdk_ld_args_t *args){
   char cmd[2048], *tuim_home, **str;

   sprintf(
      cmd, 
      "ld.lld --oformat=elf "
      "--image-base=0x0 -z separate-code -z separate-loadable-segments "
      // Disable non standard ELF features
      "--hash-style=sysv " // do not use SHT_GNU_HASH
      "-z nognustack " // do not use PT_GNU_STACK
      "-z norelro " // do not use PT_GNU_RELRO
      "--no-eh-frame-hdr " // do not use PT_GNU_EH_FRAME
   );

   if(args->shared){
      sprintf(cmd, "%s --shared -Bsymbolic", cmd);
   }else{
      sprintf(
         cmd,
         "%s -e %s -dynamic-linker /usr/bin/tuim",
         cmd, args->entry
      );
   }

   tuim_home = getenv("TUIM_HOME");
   if(tuim_home != NULL){
      sprintf(cmd, "%s --nostdlib -L '%s/lib'", cmd, tuim_home);
   }

   str = args->library;
   while(*str != NULL){
      sprintf(cmd, "%s -l%s", cmd, *str);
      ++str;
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
