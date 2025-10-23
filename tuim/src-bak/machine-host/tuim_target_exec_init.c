/* *****************************************************************************
   Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>

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
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <tuim.h>
#include "asm.h"
/* ------------------------------------
   This function execute the code at the address on the target machine --- in
   that case the host machine itself.
   * Part of Tuim Project.
   * Last modified: July 31, 2025.
------------------------------------ */

#define DEBUG 0

int tuim_target_exec_init(void *ptr, char *const argv[], char *const envp[]){
   int tuim_target_registers(void *ptr, void *new, void *old);
   int tuim_target_write(void*,tuim_addr,const void*,tuim_size);
   extern const uint64_t tuim_target_nullptr;

   char * const *tmp;
   struct_regs regs;
   uintptr_t addr, null;
   size_t len;
   int argc, argc_;

   #if DEBUG
      int printf(const char *, ...), puts(const char *);
      printf("Initializing stack\n");
   #endif

   tuim_target_registers(ptr, NULL, regs);
   addr = regs[stack_pointer];

   #if DEBUG
      printf("   Stack top: 0x%X\n", regs[stack_pointer]);
   #endif

   /* copy enrironment strings */

   for(tmp = envp; *tmp != NULL; ++tmp){
      len = strlen(*tmp) + 1;
      regs[stack_pointer] -= len;
      tuim_target_write(ptr, regs[stack_pointer], *tmp, len);

      #if DEBUG
         printf("      0x%X: %s\n", regs[stack_pointer], *tmp);
      #endif
   }

   /* copy argument strings */

   for(argc = 0, tmp = argv; *tmp != NULL; ++tmp, ++argc){}

   for(argc_ = argc-1; argc_ >= 0; --argc_){
      tmp = argv + argc_;
      len = strlen(*tmp) + 1;
      regs[stack_pointer] -= len;
      tuim_target_write(ptr, regs[stack_pointer], *tmp, len);

      #if DEBUG
         printf("      0x%X: %s\n", regs[stack_pointer], *tmp);
      #endif
   }

   regs[stack_pointer] &= ~(uintptr_t)(pointer_size - 1);

   /* write environment pointers */

   null = (uintptr_t)tuim_target_nullptr;
   regs[stack_pointer] -= pointer_size;
   tuim_target_write(ptr, regs[stack_pointer], &null, pointer_size);

   #if DEBUG
      printf("      0x%X: 0x%X\n", regs[stack_pointer], null);
   #endif

   for(tmp = envp; *tmp != NULL; ++tmp){
      len = strlen(*tmp) + 1;
      addr -= len;
      regs[stack_pointer] -= pointer_size;
      tuim_target_write(ptr, regs[stack_pointer], &addr, pointer_size);

      #if DEBUG
         printf("      0x%X: 0x%X\n", regs[stack_pointer], addr);
      #endif
   }

   /* write argument pointers */

   regs[stack_pointer] -= pointer_size;
   tuim_target_write(ptr, regs[stack_pointer], &null, pointer_size);

   #if DEBUG
      printf("      0x%X: 0x%X\n", regs[stack_pointer], null);
   #endif

   argc_ = argc;
   for(argc_ = argc-1; argc_ >= 0; --argc_){
      tmp = argv + argc_;
      len = strlen(*tmp) + 1;
      addr -= len;
      regs[stack_pointer] -= pointer_size;
      tuim_target_write(ptr, regs[stack_pointer], &addr, pointer_size);

      #if DEBUG
         printf("      0x%X: 0x%X\n", regs[stack_pointer], addr);
      #endif
   }

   addr = argc;
   regs[stack_pointer] -= pointer_size;
   tuim_target_write(ptr, regs[stack_pointer], &addr, pointer_size);

   #if DEBUG
      printf("      0x%X: 0x%X\n", regs[stack_pointer], addr);
   #endif

   #if DEBUG
      printf("   Stack: 0x%X\n", regs[stack_pointer]);
      puts("");
   #endif

   tuim_target_registers(ptr, regs, NULL);

   return TUIM_SUCCESS;
}
