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
#include <tuim.h>
#include "asm.h"
/* ------------------------------------
   This function execute the code at the address on the target machine --- in
   that case the host machine itself.
   * Part of Tuim Project.
   * Last modified: July 31, 2025.
------------------------------------ */

int tuim_target_exec_fini(void *ptr, int *exit_status){
   int tuim_target_registers(void *ptr, void *new, void *old);
   int tuim_target_read(void*,void*,tuim_addr,tuim_size);

   struct_regs regs;
   intptr_t val;

   tuim_target_registers(ptr, NULL, regs);
   tuim_target_read(ptr, &val, regs[stack_pointer], pointer_size);
   *exit_status = (int)val;

   return TUIM_SUCCESS;
}
