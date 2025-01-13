/* *****************************************************************************
   MIT License

   Copyright (c) 2024 I.F.F. dos Santos <ismaellxd@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the “Software”), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
***************************************************************************** */
#include "elf.h"
#include "tuim.h"
#include "tuim_private.h"
/* ------------------------------------
   Function to execute an already loaded ELF.
   * Part of tuim project.
   * Last modified: January 13, 2025.
------------------------------------ */
static bool exec_success;
static void exec_void(void *f);
static int exec_main(void* main, int argc, char **argv);

void* tuim_exec(unsigned type, unsigned flags, tuim_elf *elf, void *args){
   void *entry;

   /* Only execute executables */
   if(ELF_E_TYPE(*((Elf_Ehdr*)(elf->ehdr))) != ET_EXEC){
      tuim_errno = TUIM_ENOTEXEC;
      return NULL;
   }

   /* The entry point */
   entry = tuim_getentry(elf);
   if(entry == NULL){
      tuim_errno = TUIM_EENTRY;
      return NULL;
   }

   exec_success = false;
   if(type == TUIM_EXEC_TNONE){
      /* For the none type the entry point takes execution without
         setting parameters and without expecting return values. */
      exec_void(entry);
      if(exec_success){
         return entry;
      }else{
         return NULL;
      }
   }else if(type == TUIM_EXEC_TC){
      /* For the C type the entry point is interpreted as the address
         of C's main function.
         int main(int argc, char **argv); */
      typedef struct args { int argc; char **argv; } * args_p;
      static int ret;

      if(tuim_findsymbol("main", elf) != entry){
         tuim_errno = TUIM_EENTRY;
         return NULL;
      }

      ret =
      exec_main(entry, ((args_p)args)->argc, ((args_p)args)->argv);

      if(exec_success)
         return &ret;
      else{
         return NULL;
      }
   }

   /* Never return from here */
   return NULL;
}

#if /* For systems in wich POSIX API is available ------------------------- */ \
   defined (__unix__) || \
   (defined (__APPLE__) && defined (__MACH__))
/* Using POSIX API, fork this process and execute the program on the child. */
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include <signal.h>
static void exec_void(void *f){
   pid_t	pid;
   pid = fork();
   if(pid == (pid_t)0){ /* Child: The program execution process */
      ((void(*)(void))f)();
      exit(0); /* Terminate the proccess */
   }else if(pid > (pid_t)0){ /* Parent: Wait until the program terminates */
      int status;
      waitpid(pid, &status, 0);
      if(WIFEXITED(status)){
         exec_success = true;
      }else{
         /* TODO: Handle possible errors */
      }
   }
}

static int exec_main(void* main, int argc, char **argv){
   pid_t	pid;
   pid = fork();
   if(pid == (pid_t)0){ /* Child: The program execution process */
      exit(((int(*)(int,char**))main)(argc, argv));
   }else if(pid > (pid_t)0){ /* Parent: Wait until the program terminates */
      int status;
      waitpid(pid, &status, 0);
      if(WIFEXITED(status)){
         exec_success = true;
         return WEXITSTATUS(status);
      }else{
         /* TODO: Handle possible errors */
      }
   }

   /* FIXME: Nerver return from here. */
   return 127;
}
#else
static void exec_void(void *f){
   ((void(*)(void))f)();
   exec_success = true;
}

static int exec_main(void* main, int argc, char **argv){
   int ret = ((int(*)(int,char**))main)(argc, argv);
   exec_success = true;
   return ret;
}
#endif
