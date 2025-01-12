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
***************************************************************************** */#include "tuim.h"
#include "elf.h"
#include "tuim_private.h"
/* ------------------------------------
   Function to load in memory a PIE ELF file.
   * Part of tuim project.
   * Last modified: January 12, 2025.
------------------------------------ */

void* tuim_exec(unsigned type, unsigned flags, tuim_elf *elf, void *args){
   pid_t	pid;
   void *entry, *return_code;
   static int c_ret;
   size_t size;
   bool return_int;

   if(ELF_E_TYPE(*((Elf_Ehdr*)(elf->ehdr))) != ET_EXEC){
      tuim_errno = TUIM_ENOTEXEC;
      return NULL;
   }

   return_int = ((type == TUIM_EXEC_TNONE) || (type == TUIM_EXEC_TC));

   /* Allocate a shared memory */
   if(return_int)
      size = sizeof(int);
   else{
      tuim_errno = TUIM_EUNKNOWNTYPE;
      return NULL;
   }
   return_code = malloc(size);
   if(return_code == NULL){
      tuim_errno = TUIM_EMEMORY;
      return NULL;
   }

   /* The entry point */
   entry = tuim_getentry(elf);
   if(entry == NULL){
      tuim_errno = TUIM_EENTRY;
      return NULL;
   }
   if((type == TUIM_EXEC_TC) && (tuim_findsymbol("main", elf) != entry)){
      tuim_errno = TUIM_EENTRY;
      return NULL;
   }

   /* Create a separete process for the program execution */
   pid = fork();
   if(pid == (pid_t)0){
      if(type == TUIM_EXEC_TNONE){
         /* Simply jump to the entry point */
         ((void(*)(void))entry)();
         *((int*)return_code) = 0;
      }else if(type == TUIM_EXEC_TC){
         struct args { int argc; char **argv; } *args_;
         void *exit;

         /* Call C's main function */
         args_ = args;
         *((int*)return_code) =
         ((int(*)(int,char**))entry)(args_->argc, args_->argv);

         /* Call C's exit function */
         exit = tuim_findsymbol("exit", elf);
         if(exit != NULL){
            ((void(*)(int))exit)(0);
         }
      }
      /* Terminate this proccess, if alive */
      exit(*((int*)return_code));
   }else if(pid > (pid_t)0){
      int status;
      waitpid(pid, &status, 0);
      /* TODO: Handle possible errors */
   }else{
      tuim_errno = TUIM_EPROC;
      return NULL;
   }

   if(return_int){
      static int tmp;
      tmp = *((int*)return_code);
      free(return_code);
      return_code = &tmp;
   }

   return return_code;
}
