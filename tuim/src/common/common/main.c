/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 03, 2025.
 ******************************** MIT licensed ********************************/
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tuim.h>

#ifndef BUFFER_SIZE
   #define BUFFER_SIZE 1024
#endif

static void *environment(const char *env);

/*******************************************************************************
 * This is the reference implementation for the tuim CLI.
 *
 * For a detailed description see the appendex "Command Line Interface" of
 * "Tuim: A Portable Application Binary Interface".
 ******************************************************************************/
int main(int argc, char **argv){
   int status;
   const char *strerror;
   const char *backend = NULL;
   char **envp;
   tuim_ctx *ctx;

   #ifdef NDEBUG
      (void)puts("NDEBUG defined")
   #endif

   /**
    * The following environment variables affect the behavior of this program:
    * TUIM_HOME            Change where find executable and libraries
    * LD_LIBRARY_PATH      Change where find libraries
    *
    * You shall refer to "Tuim: A Portable Application Binary Interface" and
    * to "System V Application Binary Interface" for more details.
    */
   const char *tuim_home = environment("TUIM_HOME");
   const char *ld_library_path = environment("LD_LIBRARY_PATH");

   /**
    * The first argument without - (single hyphen) shall be considered the
    * executable name or executable path.
    *
    * At the end of this for(;;) the argv variable shall be ready to be passed
    * to TUIM_EXEC.
    */
   for(--argc, ++argv; ; --argc, ++argv){
      if(*argv == NULL) return EXIT_SUCCESS;
      if(**argv != '-') break;
      if(*argv[1] == 'b'){
         --argc, ++argv;
         backend = *argv;
      }
   }

   envp = NULL;

   /* CREATE A EXECUTION CONTEXT */
   status = tuim(TUIM_NEWCONTEXT, backend, tuim_home, ld_library_path, &ctx);
   if(status != TUIM_SUCCESS) goto on_error;

   /* LOAD EXECUTABLE */
   status = tuim(TUIM_LOAD, ctx, *argv);
   if(status != TUIM_SUCCESS) goto on_error;

   /* EXECUTE */
   status = tuim(TUIM_EXEC, ctx, *argv, argv, envp);
   if(status != TUIM_SUCCESS) goto on_error;

   status = tuim(TUIM_DELETECONTEXT, ctx);
   if(status != TUIM_SUCCESS) goto on_error;

   return status;

   on_error:
   tuim(TUIM_STRSTATUS, ctx, &strerror);
   fprintf(stderr, "tuim: %s\n", strerror);
   return status;
}

/*******************************************************************************
 * Function to get the value of a environment variable.
 *
 * The env variable shall point to a null terminated string with the desired
 * environment variable.
 *
 * The function return a pointer to a copy of the environment variable value.
 ******************************************************************************/
static void *environment(const char *env){
   static char memory_buffer[BUFFER_SIZE];
   static size_t stack = 0;
   char *value;
   size_t len;

   value = getenv(env);
   if(value == NULL) return NULL;
   len = strlen(value) + 1;
   if(stack + len > BUFFER_SIZE) return NULL;

   strcpy(memory_buffer + stack, value);
   value = memory_buffer + stack;
   stack += len;

   return value;
}
